##
# class that prints some of the meta alerts content
class EmptyPythonTrigger(trigger.BaseWrapper):
  ##
  # implementation of virtual call
  def triggerImpl(self, ma):
    print("")
    print("ID: "+ma.get("metaalert.id").get())
    print("Name: "+ma.get("metaalert.name").get())
    print("Children: "+ma.get("metaalert.children.size").get())
    print("")

# return instance to the C++ code
derived=EmptyPythonTrigger()
