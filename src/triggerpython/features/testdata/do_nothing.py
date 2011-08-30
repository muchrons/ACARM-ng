# class definition
class TestPythonTrigger(trigger.BaseWrapper):
  def triggerImpl(self, ma):
    print("id is: "+ma.get("metaalert.id"))

# return instance to the C++ code
derived=TestPythonTrigger()
