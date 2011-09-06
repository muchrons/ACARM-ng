##
# class that does nothing
class EmptyPythonFilter(filter.BaseWrapper):

  ##
  # check for given entry
  def isEntryInterestingImpl(self, ma, data):
    return True

  ##
  # get name for new meta-alert
  def getMetaAlertNameImpl(self, maThis, thisData, maOther, otherData):
    return "hitchhiker's guide to the galaxy"

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
derived=EmptyPythonFilter()
