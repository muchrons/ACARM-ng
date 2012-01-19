##
# class that does nothing
class EmptyPythonTrigger(triggerapi.BaseWrapper):
  ##
  # implementation of virtual call
  def triggerImpl(self, ma):
    pass # means empty method

# return instance to the C++ code
derived=EmptyPythonTrigger()
