lib = File.expand_path("../lib", __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require "win_print/version"

Gem::Specification.new do |spec|
  spec.name          = "win_print"
  spec.version       = WinPrint::VERSION
  spec.authors       = ["Martin Schuster"]
  spec.email         = "schuster@schul-logistik.de"
  spec.homepage      = "http://github.com/mrschuster/win_print"
  spec.license       = "MIT"
  spec.summary       = "Gem for raw printing on windows"

  spec.test_files    = Dir['test/*']
  spec.extensions    = ["ext/extconf.rb"]
  spec.files         = ['Rakefile', 'win_print.gemspec', 'Gemfile'] + Dir["ext/*.c", "ext/*.h", "ext/*.rb", "lib/**/*"]

  spec.add_development_dependency "bundler", ">=1.16"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "minitest"

  spec.description = <<-EOL
    The WinPrint library offers a way to send raw data to
    a system printer on windows. It also allows to fetch the
    printer list.
  EOL
end
