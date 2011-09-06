##
# class that rises execption in c-tor
class ExceptionalPythonFilter(filterapi.FilterBaseWrapper):

  ##
  # create object
  def __init__(self):
    filterapi.FilterBaseWrapper.__init__(self)
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

  ##
  # return some data for a given node
  def makeThisEntryUserDataImpl(self, ma):
    return None

  ##
  # return some data for new node
  def makeUserDataForNewNodeImpl(self, maThis, thisData, maOther, otherData, maNewNode):
    return None


# return instance to the C++ code
derived=ExceptionalPythonFilter()
