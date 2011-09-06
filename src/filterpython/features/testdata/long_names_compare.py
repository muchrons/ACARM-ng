import re

##
# class that does nothing
class LongNamesComparePythonFilter(filterapi.FilterBaseWrapper):

  ##
  # create object
  def __init__(self):
    filterapi.FilterBaseWrapper.__init__(self)
    ## prefix for meta-alerts' names
    self.prefix="[python] /script/ "

  ##
  # check for given entry
  def isEntryInterestingImpl(self, ma, data):
    if(data!=None):
      raise Exception("data is magicially set: 1")
    return len( ma.get("metaalert.name").get() )>5

  ##
  # get name for new meta-alert
  def getMetaAlertNameImpl(self, maThis, thisData, maOther, otherData):
    if(thisData!=None):
      raise Exception("data is magicially set:2")
    if(otherData!=None):
      raise Exception("data is magicially set:3")

    s=maThis.get("metaalert.name").get();
    return self.prefix+s

  ##
  # test for correlation possibility
  def canCorrelateImpl(self, maThis, thisData, maOther, otherData):
    if(thisData!=None):
      raise Exception("data is magicially set:2")
    if(otherData!=None):
      raise Exception("data is magicially set:3")

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

  ##
  # return some data for new node
  def makeUserDataForNewNodeImpl(self, maThis, thisData, maOther, otherData, maNewNode):
    return None


# return instance to the C++ code
derived=LongNamesComparePythonFilter()
