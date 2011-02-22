#!/usr/bin/env ruby

require 'flash'
require 'gtk2'

DefaultPaths = %w[
  /usr/lib/mozilla/plugins/libflashplayer.so
  /usr/lib/mozilla-firefox/plugins/libflashplayer.so
  %h/.mozilla/plugins/libflashplayer.so
  %h/.mozilla/firefox/plugins/libflashplayer.so
]

def usage
  $stderr.puts "usage: #{$0} file.swf [/path/to/libflashplayer.so]"
  exit 1
end

def callback(file, type)
  case type
  when Flash::File::EVENT_PLAYBACK_STOPPED
    puts "plugin has stopped playing flash file #{file}"
    Gtk::main_quit
  end
end

swf_file_path = ARGV.shift
swf_file_path.nil? && usage
plugin_path = ARGV.shift || DefaultPaths.find{|x| File.exists?(x.gsub(/%h/, ENV['HOME']))}
plugin_path.nil? && usage
puts "using Flash/Ruby v#{Flash::VERSION}"
puts "using plugin from #{plugin_path}"
plugin_library = Flash::Library.new(plugin_path)
puts "description: #{plugin_library.description}"
swf_file = Flash::File.new(plugin_library, swf_file_path, method(:callback))

Gtk::init
window = Gtk::Window.new(Gtk::Window::TOPLEVEL)
window.set_default_size(640, 480)
window.signal_connect("delete-event") {Gtk::main_quit}
window.show
swf_file.play(window)
Gtk::main
