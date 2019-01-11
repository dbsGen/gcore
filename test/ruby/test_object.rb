require 'gr/object'
require 'gr/callback'

class TestObject < gc::Object
  native 'TestObject'

  def call_cb
    callback.inv "InRuby"
  end

end