import os

from twisted.application import service
from buildslave.bot import BuildSlave

basedir = os.path.expanduser('~/buildbot/slave/')

# if this is a relocatable tac file, get the directory containing the TAC
if basedir == '.':
    basedir = os.path.abspath(os.path.dirname(__file__))

# note: this line is matched against to check that this is a buildslave
# directory; do not edit it.
application = service.Application('buildslave')

try:
    # let's directly use the Python logging module, we juste need to log
    # to stdout/stderr supervisor will take care to log in a file and do
    # the log rotation for us.
    import logging
    from twisted.python.log import ILogObserver, PythonLoggingObserver
    logging.basicConfig()
    application.setComponent(ILogObserver, PythonLoggingObserver().emit)
except ImportError:
    # probably not yet twisted 8.2.0 and beyond, can't set log yet
    pass

buildmaster_host = 'localhost'
port = 9989
slavename = 'debian_wheezy_x86_64'
passwd = os.environ['BUILDBOT_SLAVES_PASSWORD']
keepalive = 600
usepty = 0
umask = None
maxdelay = 300

s = BuildSlave(buildmaster_host, port, slavename, passwd, basedir,
               keepalive, usepty, umask=umask, maxdelay=maxdelay)
s.setServiceParent(application)
