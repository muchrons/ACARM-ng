configuration file format
=========================

file is an XML with sections corresponding to parsing specific settings of
components using it.
example may look like this:

<?xml version="1.0" encoding="UTF-8"?>
<acarm_ng>
  <persistency>
    <!-- persistency specific options -->
  </persistency>

  <logger>
    <!-- logger specific options -->
  </logger>

  <!-- and so on... -->
</acarm_ng>

in the following sections only subtrees specific for given components will
be shown in the example, for sake of document's simplicity.


persistency configuration
=========================

configuration of persistenyc requires only giving its type to use and options
specyfic for given type.
example:

<persistency>
  <type>PostgreSQL</type>
  <persistenyctype>
    <!-- db-scecific -->
  </persistencytype>
</data_base>

where persistencytype can be one of the following:
  * postgresql

postgresql data base configuration template looks like this:

<persistency>
  <type>PostgreSQL</type>
  <postgresql>
    <user>john</user>
    <pass>$3cr3t</pass>
    <host>db.server.my.domain.org</host>
    <port>5432</port>
  </postgresql>
</persistency>


logger configuration
====================

TBD (TODO)

