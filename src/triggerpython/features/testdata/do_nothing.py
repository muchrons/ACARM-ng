##
# class that does nothing
class EmptyPythonTrigger(triggerapi.TriggerBaseWrapper):
  ##
  # implementation of virtual call
  def triggerImpl(self, ma):
    pass # means empty method

# return instance to the C++ code
derived=EmptyPythonTrigger()
