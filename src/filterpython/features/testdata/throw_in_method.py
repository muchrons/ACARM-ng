##
# class that rises execption in all methods
class MoreExceptionalPythonFilter(filter.BaseWrapper):

  ##
  # check for given entry
  def isEntryInterestingImpl(self, ma, data):
    return True

  ##
  # get name for new meta-alert
  def getMetaAlertNameImpl(self, maThis, thisData, maOther, otherData):
    raise Exception("1: now let's see what happens next...")

  ##
  # test for correlation possibility
  def canCorrelateImpl(self, maThis, thisData, maOther, otherData):
    raise Exception("2: now let's see what happens next...")

  ##
  # return some data for a given node
  def makeThisEntryUserDataImpl(self, ma):
    return None

  ##
  # return some data for new node
  def makeUserDataForNewNodeImpl(self, maThis, thisData, maOther, otherData, maNewNode):
    raise Exception("3: now let's see what happens next...")


# return instance to the C++ code
derived=MoreExceptionalPythonFilter()
