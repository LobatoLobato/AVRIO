const fs = require("fs");
const path = "./lib/AVRIO/library.json";
const prevPath = "./lib-version-check/prev.txt";
const versionRegexp = /(?<=("version":\s*"))[0-9\.]+/;
const content = fs.readFileSync(path).toString();
const version = content.match(versionRegexp).at(0);
const prevVersion = fs.readFileSync(prevPath).toString();
if (version === prevVersion) {
  throw new Error(
    "Library version is the same as previous version, please update the version in " +
      path
  );
}
fs.writeFileSync(prevPath, version);
console.log("Library version check completed");
