param (
    [Parameter(Mandatory = $true, Position = 0)]
    [ValidateSet("major", "minor", "patch")]
    [string]$ComponentToIncrement
)

$ErrorActionPreference = "Stop"

$RootDir = "$PSScriptRoot\.."
$XmlFilePath = "$RootDir\config.xml"

Write-Output "Parsing ImageXEX configuration..."
$Xml = [xml](Get-Content $XmlFilePath)

$Major = [int]$Xml.xex.version.major
$Minor = [int]$Xml.xex.version.minor
$Patch = [int]$Xml.xex.version.qfe

Write-Output "Current version: v$Major.$Minor.$Patch"

switch ($ComponentToIncrement) {
    "major" {
        $Major += 1
        $Xml.xex.version.major = [string]$Major
    }
    "minor" {
        $Minor += 1
        $Xml.xex.version.major = [string]$Minor
    }
    "patch" {
        $Patch += 1
        $Xml.xex.version.qfe = [string]$Patch
    }
}

$NewVersionString = "v$Major.$Minor.$Patch"
Write-Output "New version: $NewVersionString"

Write-Output "Updating ImageXEX configuration with the new version number..."
$Xml.Save($XmlFilePath)

Write-Output "Committing changes made to ImageXEX configuration..."
git add "$XmlFilePath"
git commit -m "$NewVersionString"

Write-Output "Creating tag..."
git tag -m "$NewVersionString" "$NewVersionString"
