const { exec } = require("child_process");
const { readFileSync } = require("fs");
const LOCAL_PATH = "./lib/AVRIO/library.json";
const LOCAL_VERSION_REGEXP = /(?<=("version":\s*"))[0-9\.]+/;
const LOCAL_VERSION = readFileSync(LOCAL_PATH)
  .toString()
  .match(LOCAL_VERSION_REGEXP)
  .at(0);

exec("pio pkg show lobatolobato/AVRIO", (error, stdout, stderr) => {
  if (error) throw new Error(error.message);

  if (stdout) {
    const VERSIONS = stdout.match(/(?<=version(.|\n|\r)*)\d+\.\d+\.\d+/gim);
    const CURRENT_VERSION = VERSIONS.at(0);
    if (LOCAL_VERSION === CURRENT_VERSION) {
      const error = `Published version (${CURRENT_VERSION}) is the same as local version (${LOCAL_VERSION})
      Please update the version in ${LOCAL_PATH}`;
      throw new Error(error);
    } else if (LOCAL_VERSION < CURRENT_VERSION) {
      const error = `Published version (${CURRENT_VERSION}) is higher than local version (${LOCAL_VERSION})
      Please update the version in ${LOCAL_PATH}`;
      throw new Error(error);
    }
    console.log("Library version check completed");
  }
});
