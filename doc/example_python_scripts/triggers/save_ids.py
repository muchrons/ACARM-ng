##
# class that saves IDs of arriving meta alerts along with children count to the file
class EmptyPythonTrigger(triggerapi.BaseWrapper):
  ##
  # implementation of virtual call
  def triggerImpl(self, ma):
    id=ma.get("metaalert.id").get()
    if(ma.get("metaalert.isleaf").get()=="true"):
      children="none"
    else:
      children=ma.get("metaalert.children.size").get()

    root="/home/acarm-ng/acarm-ng-inst/var/log/acarm-ng/"
    file=open(root+"saved_ids.txt", "a")
    file.write(id+": "+children+"\n")
    file.close()

# return instance to the C++ code
derived=EmptyPythonTrigger()
