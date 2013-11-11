import os

from twisted.application import service
from buildbot.master import BuildMaster

basedir = os.path.expanduser('~/buildbot/master')

# Default umask for server
umask = None

# if this is a relocatable tac file, get the directory containing the TAC
if basedir == '.':
    basedir = os.path.abspath(os.path.dirname(__file__))

# note: this line is matched against to check that this is a buildmaster
# directory; do not edit it.
application = service.Application('buildmaster')

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

configfile = r'master.cfg'

m = BuildMaster(basedir, configfile)
m.setServiceParent(application)
