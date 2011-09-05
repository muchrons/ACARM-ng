import re

##
# class that does nothing
class LongNamesComparePythonFilter(filter.BaseWrapper):

  ##
  # create object
  def __init__(self):
    filter.BaseWrapper.__init__(self)
    self.prefix="[python] /script/ "

  ##
  # check for given entry
  def isEntryInterestingImpl(self, ma, data):
    return len( ma.get("metaalert.name").get() )>5

  ##
  # get name for new meta-alert
  def getMetaAlertNameImpl(self, maThis, thisData, maOther, otherData):
    s=maThis.get("metaalert.name").get();
    return self.prefix+s

  ##
  # test for correlation possibility
  def canCorrelateImpl(self, maThis, thisData, maOther, otherData):
    s1 =maThis.get("metaalert.name").get()
    s2 =maOther.get("metaalert.name").get()
    reg=re.escape(self.prefix)
    st1=re.sub(reg, "", s1)
    st2=re.sub(reg, "", s2)
    return st1==st2

  ##
  # return some data for a given node
  def makeThisEntryUserDataImpl(self, ma):
    return None


# return instance to the C++ code
derived=LongNamesComparePythonFilter()
