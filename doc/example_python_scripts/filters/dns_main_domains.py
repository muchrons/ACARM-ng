import re

##
# saved DNS name of a given host
class DNSFilterData(filterapi.DataWrapper):

  ##
  # create object
  def __init__(self, dns):
    filterapi.DataWrapper.__init__(self)
    ## dns name to save
    self.dns=dns

  ##
  # gets saved DNS name
  def getDNS(self):
    return self.dns


##
# correlate toghether elements with the same DNS names of source hosts
class SameDNSRootPythonFilter(filterapi.BaseWrapper):

  ##
  # build and save regexp
  def __init__(self):
    filterapi.BaseWrapper.__init__(self)
    ## compile regexp saving only root 2 names
    self.reg=re.compile("^.*\\.([^\\.]+\\.[^\\.]+)$")

  ##
  # check for given entry
  def isEntryInterestingImpl(self, ma, data):
    return data!=None;

  ##
  # get name for new meta-alert
  def getMetaAlertNameImpl(self, maThis, thisData, maOther, otherData):
    return "[python] from " + thisData.getDNS()

  ##
  # test for correlation possibility
  def canCorrelateImpl(self, maThis, thisData, maOther, otherData):
    return thisData.getDNS()==otherData.getDNS()

  ##
  # create data for a given node
  def makeThisEntryUserDataImpl(self, ma):
    if(ma.get("metaalert.isleaf").get()=="false"):
      return None
    if(ma.get("metaalert.alert.sources.size").get()!="1"):
      return None
    dns=ma.get("metaalert.alert.sources.0.name").get()
    if(dns==None):
      return None
    dns=self.reg.sub("\\1", dns) # limit to 2 root parts of the domain
    return DNSFilterData(dns)

  ##
  # return some data for new node
  def makeUserDataForNewNodeImpl(self, maThis, thisData, maOther, otherData, maNewNode):
    return thisData


# return instance to the C++ code
derived=SameDNSRootPythonFilter()
