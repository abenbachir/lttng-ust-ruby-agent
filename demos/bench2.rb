# frozen-string-literal: true
require 'benchmark'
require 'memory_profiler'
require 'rexml/document'
require "nokogiri"

# use strings less than 23 characters
# Make key as freeze in output.rb:267 => def emit(tag, es, chain, key="".freeze)
# issue with function get_azure_resid_from_imds, oms_configuration.rb:216, it's allocate a lots of objects
#
# This lib REXML allocating a lots of strings like : "", "<", ">", "\n", and slow,  should be replated by libxml or nokogiri
# http://www.rubyinside.com/ruby-xml-performance-benchmarks-1641.html
# Affected files :
# plugins/out_oms_blob.rb
# plugins/agent_topology_request_script.rb
# plugins/filter_patch_management.rb
# plugins/filter_changetracking.rb
# plugins/filter_linux_update_run_progress.rb
# plugins/change_tracking_runner.rb
# plugins/patch_management_lib.rb
# plugins/changetracking_lib.rb
# plugins/collectmcafeeinfo.rb
# plugins/npmd_config_lib.rb
# plugins/out_oms_changetracking_file.rb
#
if "1".frozen?
  mode = "frozen-string true"
else
  mode = "frozen-string false"
end
puts mode

puts -"foo"
hash = {}
hash['key1'] = 123
p "a".object_id, "a".object_id, "a".object_id

# GC.start
# GC.disable
# 1_000_000.times { 'a' }


RUBY_DIR = 'RUBY_DIR/'
TAILSCRIPT = 'TAILSCRIPT/'
path = '/tmp/test.log'
pos_file = 'pos_file/'.freeze
command = nil

N = 50

blocks_committed = Array.new(N) { "0-" * 25 }
Benchmark.bm{|x|
  # x.report('simple literal'){
  #   N.times{ 'a' }
  # }
  # x.report('[""]'){
  #   N.times{ hash["key1"] }
  # }
  # x.report('.dup'){
  #   N.times{ 'foo dup'.dup }
  # }
  # x.report('+@'){
  #   N.times{ +'foo' }
  # }
  # x.report('.freeze'){
  #   N.times{ 'foo freeze'.freeze }
  # }
  # x.report('-@'){
  #   N.times{ -'foo' }
  # }
  x.report("Nokogiri ") {
    builder = Nokogiri::XML::Builder.new do |xml|
      xml.BlockList {
        blocks_committed.each { |blockid| xml.Committed blockid }
      }
    end
    commit_msg = builder.to_xml(:save_with => Nokogiri::XML::Node::SaveOptions::AS_XML + Nokogiri::XML::Node::SaveOptions::NO_DECLARATION)
    # puts commit_msg
  }
  x.report("REXML ") {
    doc = REXML::Document.new "<BlockList />"
    blocks_committed.each { |blockid| doc.root.add_element(REXML::Element.new("Committed").add_text(blockid)) }
    commit_msg = doc.to_s
    # puts commit_msg
  }
  # x.report('dup.upcase'){
  #   N.times{ 'foo freeze'.dup.upcase }
  # }
  # x.report('dup.upcase!'){
  #   N.times{ 'foo freeze'.dup.upcase! }
  # }
  # x.report('String.new.upcase'){
  #   N.times{ String.new('foo freeze').upcase! }
  # }
}


# Benchmark.bm do |x|
#   27.times do |i|
#     x.report(i) {
#       N.times{'A' * i }
#     }
#   end
# end
# N.times{ 'foo' }
key = "#{pos_file}".freeze
keys_upcase = {"key": "ABCD"}

# MemoryProfiler.start
# 1000.times{ 'dup.upcase'.dup.upcase }
# 1000.times{ 'dup.upcase!'.dup.upcase!.freeze }
# 1000.times{ String.new('String.new.upcase!').upcase!.freeze }
# report = MemoryProfiler.stop
# report.pretty_print