##
# exception raising implementation
class RaiserPythonTrigger(triggerapi.BaseWrapper):
  ##
  # virtual call implementation - throws when called
  def triggerImpl(self, ma):
    raise Exception("some error risen in the script for tests")

# return instance to the C++ code
derived=RaiserPythonTrigger()
