##
# class that saves IDs of arriving meta alerts along with children count to the file
class EmptyPythonTrigger(triggerapi.BaseWrapper):
  ##
  # implementation of virtual call
  def triggerImpl(self, ma):
    id=ma.get("metaalert.id").get()
    name=ma.get("metaalert.name").get()

    root="/home/acarm-ng/acarm-ng-inst/var/log/acarm-ng/"
    file=open(root+"saved_names.txt", "a")
    file.write(id+": "+name+"\n")
    file.close()

# return instance to the C++ code
derived=EmptyPythonTrigger()
