begin
  require 'mkmf-gnome2'
rescue
  $stderr.puts "ERROR: you do not have the Ruby Glib2 bindings installed"
  exit 1
end

PKGConfig.have_package('libflash-1.0')

%w[flash_file_new flash_library_new flash_file_resize].each do |f|
  unless have_func(f)
    $stderr.puts "\nERROR: missing function #{f}, aborting."
    exit 1
  end
end

debug = ARGV.shift
if debug == "--debug"
  $CPPFLAGS << " -DRFLASH_DEBUG"
  $CFLAGS << " -g"
end

create_makefile('flash')
