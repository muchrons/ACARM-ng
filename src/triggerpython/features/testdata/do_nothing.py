print("PYTHON: BEGIN")
from trigger import *

class PythonTrigger(BaseWrapper):
  def triggerImpl(self, ma):
    print("id is: "+ma.get("metaalert.id"))

derived=PythonTrigger()

print("PYTHON: END")
