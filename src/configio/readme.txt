configuration file format
=========================

file is an XML with sections corresponding to parsing specific settings of
components using it.
example may look like this:

<?xml version="1.0" encoding="UTF-8"?>
<acarm_ng>
  <data_base>
    <!-- data base specific options -->
  </data_base>

  <logger>
    <!-- logger specific options -->
  </logger>

  <!-- and so on... -->
</acarm_ng>

in the following sections only subtrees specific for given components will
be shown in the example, for sake of document's simplicity.


data base configuration
=======================

configuration of data base requires only giving data base type and options
specyfic for given type.
example:

<data_base>
  <type>PostgreSQL</type>
  <dbtype>
    <!-- db-scecific -->
  </dbtype>
</data_base>

where dbtype can be one of the following:
  * postgresql

postgresql data base configuration template looks like this:

<data_base>
  <type>PostgreSQL</type>
  <postgresql>
    <user>john</user>
    <pass>$3cr3t</pass>
    <host>db.server.my.domain.org</host>
    <port>5432</port>
  </postgresql>
</data_base>


logger configuration
====================

TBD (TODO)

