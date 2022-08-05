<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
  xmlns="http://www.w3.org/1999/xhtml"
  xmlns:rss="http://purl.org/rss/1.0/"
  xmlns:dc="http://purl.org/dc/elements/1.1/"
  exclude-result-prefixes="rdf rss dc"
>
<xsl:template match="/">
  <xsl:apply-templates select="rdf:RDF"/>
</xsl:template>

<xsl:template match="rdf:RDF">
  <html xml:lang="ja" lang="ja">
   <xsl:apply-templates select="rss:channel"/>
   <body>
    <p>Cell Speed Challenge 2008</p>
    <ul class="items">
     <xsl:apply-templates select="rss:item"/>
    </ul>
    <hr />
    <address>(c) 2007,2008 Cell Speed Challenge 2008 Committee</address>
   </body>
  </html>
</xsl:template>

<xsl:template match="rss:channel">
  <head>
   <title>RDF of <xsl:value-of select="rss:title"/></title>
   <link rel="stylesheet" href="./cell.css" type="text/css" />
   <link rel="shortcut icon" href="../img/favicon.ico" />
  </head>
</xsl:template>

<xsl:template match="rss:item">
  <li>
   <a>
    <xsl:attribute name="href">
     <xsl:value-of select="rss:link"/>
    </xsl:attribute>
    <xsl:value-of select="rss:title"/>
   </a>
   <span class="rdfdate"> (<xsl:value-of select="dc:date"/>)</span><br />
   <p><xsl:value-of select="rss:description"/></p>
  </li>
</xsl:template>

<xsl:template match="rss:image">
 <h1>
  <a>
   <xsl:attribute name="href">
    <xsl:value-of select="rss:link"/>
   </xsl:attribute>
   <img>
    <xsl:attribute name="src">
     <xsl:value-of select="rss:url"/>
    </xsl:attribute>
    <xsl:attribute name="alt">
     <xsl:value-of select="rss:title"/>
    </xsl:attribute>
   </img>
  </a>
 </h1>
</xsl:template>

</xsl:stylesheet>
