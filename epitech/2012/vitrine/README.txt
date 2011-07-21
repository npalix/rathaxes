To generate the website read the following howto and then instal jekyll using:

   gem instal jekyll

Finally, run "jekyll --server --base-url /2012/rathaxes" and point your browser
to http://locahost:4000/2012/rathaxes to see the result.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

How to setup rubygems into your home
====================================

I'm going to explain how to install a rubygems environment into your home in a
single hidden directory. This has several advantages:

- You do not pollute your system (everybody know how shitty are ruby packages);
- You don't depends on your packages system (since Debian developers gave up on
  the ruby diarrhea);
- You can always install the bleeding edge stuff (and we know that ruby is
  always bleeding);
- If anything goes wrong you just have to remove a directory.

The only pre-requisites are:

- ruby (if your using ruby1.9.1 on Debian/Ubuntu make yourself a favor and do
  the right alias to *ruby*);
- ruby-dev and build-essential (to build ruby extensions);
- the latest rubygems tarball from http://rubygems.org/.

Install rubygems
----------------

Use the following commands::

   $ mkdir -p ~/.ruby-env/gems/
   $ tar -zxf <RUBYGEMS_TARBALL>
   $ cd <RUBYGEMS>
   $ ruby setup.rb --no-rdoc --no-ri --prefix=$HOME/.ruby-env/

Setup the right environment variables
-------------------------------------

Add the following environment variables into your shell startup script (e.g.
.profile):

.. code-block:: sh

   export GEM_HOME=$HOME/.ruby-env/gems
   export RUBYLIB=$HOME/.ruby-env/lib
   PATH="$HOME/.ruby-env/bin/:$GEM_HOME/bin/:$PATH"

Again, add an alias for *gem* if needed.

Maintenance
-----------

To uninstall all your gems run the following command::

   $ rm -rf $GEM_HOME

To upgrade rubygems to the latest version run the following command::

   $ gem update --system

See Also
--------

- http://docs.rubygems.org/read/chapter/3#page83
