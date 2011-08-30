# class definition
class TestPythonTrigger(trigger.BaseWrapper):
  def triggerImpl(self, ma):
    ma=None
    #print("id is: "+ma.get("metaalert.id").get())

# return instance to the C++ code
derived=TestPythonTrigger()
