require 'test_object'

$to = TestObject.create

$to.int_value = 333

class ClassA

  def get_number
    2018
  end
end

$objct_a = ClassA.new