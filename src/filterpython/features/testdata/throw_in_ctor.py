##
# class that rises execption in c-tor
class ExceptionalPythonFilter(filter.BaseWrapper):

  ##
  # create object
  def __init__(self):
    filter.BaseWrapper.__init__(self)
    raise Exception("now let's see what happens next...")

  ##
  # check for given entry
  def isEntryInterestingImpl(self, ma, data):
    return False

  ##
  # get name for new meta-alert
  def getMetaAlertNameImpl(self, maThis, thisData, maOther, otherData):
    return "The Answer is 42"

  ##
  # test for correlation possibility
  def canCorrelateImpl(self, maThis, thisData, maOther, otherData):
    return False


# return instance to the C++ code
derived=ExceptionalPythonFilter()
