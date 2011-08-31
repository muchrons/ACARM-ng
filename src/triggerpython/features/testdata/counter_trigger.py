# class definition
class CounterPythonTrigger(trigger.BaseWrapper):

  def __init__(self):
    trigger.BaseWrapper.__init__(self)
    self.counter_=0

  def triggerImpl(self, ma):
    ++self.counter_

# return instance to the C++ code
derived=CounterPythonTrigger()
