##
# class that does nothing
class EmptyPythonFilter(filter.BaseWrapper):
  ##
  # check for given entry
  def isEntryInterestingImpl(self, ma, data):
    return true;

# return instance to the C++ code
derived=EmptyPythonFilter()
