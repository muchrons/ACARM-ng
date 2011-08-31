# class definition
class EmptyPythonTrigger(trigger.BaseWrapper):
  def triggerImpl(self, ma):
    pass # means empty method
    #print("id is: "+ma.get("metaalert.id").get())

# return instance to the C++ code
derived=EmptyPythonTrigger()
