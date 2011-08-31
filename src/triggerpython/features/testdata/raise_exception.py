# class definition
class RaiserPythonTrigger(trigger.BaseWrapper):
  def triggerImpl(self, ma):
    raise Exception("some error risen in the script for tests")

# return instance to the C++ code
derived=RaiserPythonTrigger()
