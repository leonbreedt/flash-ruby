#
# Gem specification file
# See http://rubygems.rubyforge.org
#

require 'rubygems'

spec = Gem::Specification.new do |s|
  s.name = 'flash'
  s.version = '0.1.0'

  s.summary = 'Ruby bindings for libflash.'
  s.description = <<-EOF
  Ruby bindings for libflash.

  This module allows Ruby GTK programs to perform playback of Flash files using
  the Macromedia(R) Flash plugin for Mozilla.
  EOF
  s.requirements << "The libflash library"
  s.requirements << "Ruby GTK bindings"

  s.files = Dir.glob("{examples,ext,lib,test}/**/*")
  s.files.concat %w[AUTHORS ChangeLog COPYING README]

  s.extensions << "ext/extconf.rb"

  s.require_path = 'lib'
  s.autorequire = 'flash'

  s.has_rdoc = false # disabled for now, needs special magic :(

  s.author = 'Leon Breedt'
  s.email = 'ljb@bitserf.org'
  s.homepage = 'https://github.com/bitserf/flash-ruby'
  s.rubyforge_project = 'flash-ruby'
end
