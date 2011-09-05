##
# class that does nothing
class InvalidPythonFilter(filter.BaseWrapper):
  ##
  # method that is never called
  def noUsed(self):
    pass


# return instance to the C++ code
derived=InvalidPythonFilter()
