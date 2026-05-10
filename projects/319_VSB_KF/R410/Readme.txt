Title: Replacing the tagged faceplate files.
Subject: Replacing the tagged faceplate files on the main line with the original(untagged) faceplate files.

we have placed the tagged faceplates on the main line for Localization purpose. And scmbld branch is created for having the original(untagged) faceplates, which are obtained by running DTU utility over that and these original(untagged) faceplates are used by the scm build.
But the modified DTU utility in viking (i.e., Release CCA 100), is capable of capturing the string which needs to be localized directly from the original faceplates(untagged). so there is no need to have the tagged faceplates on the main line. we can have the original(untagged) faceplates on the main line, which will avoid modification on the scmbld branch

The following are the set of files which will get affected:

sysdtlcda_fp.htm
sysdtldevctla_fp.htm
sysdtlpida_fp.htm
sysdtlpospa_fp.htm
sysdtlrampa_fp.htm
sysdtlscma_fp.htm
sysdtlswa_fp.htm
sysdtltima_fp.htm
sysdtltota_fp.htm
sysdtldaca_fp.htm

The bottom line is: Pick the faceplates(above mentioned files) form the main line.
There is no need to run the DTU utility on the main line to get the original(untagged) faceplates and should not be picked faceplates from the scmbld branch.
