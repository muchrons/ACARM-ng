REVOKE CONNECT ON DATABASE "acarm_ng" FROM "acarm-ng-ui";
REVOKE CONNECT ON DATABASE "acarm_ng" FROM "acarm-ng-daemon";

DROP USER "acarm-ng-ui";
DROP USER "acarm-ng-daemon";
