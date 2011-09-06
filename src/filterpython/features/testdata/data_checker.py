##
# data class holding one field with ID
class SomeTestDataImpl(filter.DataWrapper):

  ##
  # create object
  def __init__(self, id):
    filter.DataWrapper.__init__(self)
    ## id to save
    self.id=id

  ##
  # gets saved ID
  def getID(self):
    return self.id


##
# class that does nothing
class DataCheckerPythonFilter(filter.BaseWrapper):

  ##
  # check for given entry
  def isEntryInterestingImpl(self, ma, data):
    id=ma.get("metaalert.id").get()
    if( id!=data.getID() ):
      raise Exception("sanity-check-1: invalid ID in data filed - something's wrong")
    return True

  ##
  # get name for new meta-alert
  def getMetaAlertNameImpl(self, maThis, thisData, maOther, otherData):
    return "[python] DataCheckerPythonFilter"

  ##
  # test for correlation possibility
  def canCorrelateImpl(self, maThis, thisData, maOther, otherData):
    c1=maThis.get("metaalert.isleaf").get()=="true"
    if( maThis.get("metaalert.id").get()!=thisData.getID() ):
      raise Exception("sanity-check-2: invalid ID in data filed - something's wrong")

    c2=maOther.get("metaalert.isleaf").get()=="true"
    if( maOther.get("metaalert.id").get()!=otherData.getID() ):
      raise Exception("sanity-check-3: invalid ID in data filed - something's wrong")

    return c1 and c2

  ##
  # return some data for a given node
  def makeThisEntryUserDataImpl(self, ma):
    id=ma.get("metaalert.id").get()
    return SomeTestDataImpl(id)

  ##
  # return some data for new node
  def makeUserDataForNewNodeImpl(self, maThis, thisData, maOther, otherData, maNewNode):
    return None


# return instance to the C++ code
derived=DataCheckerPythonFilter()
