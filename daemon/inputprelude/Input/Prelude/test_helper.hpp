trmplate<typename T, >
class SourceWrapper
{
public:
  SourceWrapper()
  {
    if (idmef_source_new(&source_)<0)
      tut::fail("Unable to create source obcject.");
  }

  ~SourceWrapper()
  {
    idmef_source_destroy(source_);
  }

  idmef_source_t * get()
  {
    assert(ptr!=NULL);
    return source_;
  }

private:
  idmef_source_t *source_;
};
