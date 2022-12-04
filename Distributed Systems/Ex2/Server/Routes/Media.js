const fs = require('fs');
const path = require('path');

const NO_ERRORS = "NO ERRORS";

// variables
const dataPath = path.join(__dirname, '../Data/Media.json');

console.log(dataPath);

// helper methods
const readFile = (callback, returnJson = false, filePath = dataPath, encoding = 'utf8') => {
    fs.readFile(filePath, encoding, (err, data) => {
        if (err) {
            console.log(err);
        }
        if (!data) data = "{}";
        callback(returnJson ? JSON.parse(data) : data);
    });
};

const writeFile = (fileData, callback, filePath = dataPath, encoding = 'utf8') => {

    fs.writeFile(filePath, fileData, encoding, (err) => {
        if (err) {
            console.log(err);
        }

        callback();
    });
};

// validate the media info in the case of create and edit
// the function returns the errors messages NO_ERRORS string in the case of no error
const validateeMediaInfo = (requestBody, isForEdit) => {

    let requiredKeys = ["name", "picture", "director", "date", "rating", "isSeries", "series_details"];
    let additionalKeysForCreate = ["id", "actors"];

    let stringInputs = ["name", "director"];
    let additionalStringInputsForCreate = ["id"];

    let numberInputs = ["rating"];

    let booleanInputs = ["isSeries"];

    let urlInputs = ["picture"];

    let dateInputs = ["date"];

    if (isForEdit === false) {
        requiredKeys = [...requiredKeys, ...additionalKeysForCreate];
        stringInputs = [...stringInputs, ...additionalStringInputsForCreate];
    }

    let requestKeys = Object.keys(requestBody);

    let hasIllegalFields = false;
    let illegalFields = "* Illegal fields (Bad Request):\n";

    let hasMissedFields = false;
    let missedFields = "* Missed required fields (Bad Request):\n"

    let hasWrongInputs = false;
    let wrongInputs = "* Wrong Inputs (Bad Request):\n";

    let finalErrorString = "";

    requestKeys.forEach((key) => {

        if (!requiredKeys.includes(key)) {

            hasIllegalFields = true;

            illegalFields += `\t- <${key}>\n`;
        }
    });

    if (hasIllegalFields) {

        finalErrorString += illegalFields + "\n";
    }


    requiredKeys.forEach((key) => {

        if (!requestKeys.includes(key)) {

            hasMissedFields = true;

            missedFields += `\t- <${key}>\n`;
        }

    });

    if (hasMissedFields) {

        finalErrorString += missedFields + "\n";
    }


    stringInputs.forEach((key) => {

        let currString = requestBody[key];

        if ((typeof currString !== 'string') || (currString.length < 2)) {

            hasWrongInputs = true;
            wrongInputs += `\t- <${key}> has to be string with min length 2 chars\n`
        }

    });

    numberInputs.forEach((key) => {

        let currNumber = requestBody[key];

        if (isNaN(currNumber) || typeof currNumber === 'boolean' || (currNumber < 1 || currNumber > 5)) {

            hasWrongInputs = true;
            wrongInputs += `\t- <${key}> has to be a number between 1-5\n`
        }

    });

    booleanInputs.forEach((key) => {

        let currBoolean = requestBody[key];

        if (typeof currBoolean !== 'boolean') {

            hasWrongInputs = true;
            wrongInputs += `\t- <${key}> has to be boolean\n`
        }

    });

    urlInputs.forEach((key) => {

        let currUrl = requestBody[key];

        if (!isValidHttpUrl(currUrl)) {

            hasWrongInputs = true;
            wrongInputs += `\t- <${key}> has to be valid HTTP URL\n`
        }

    });

    dateInputs.forEach((key) => {

        let currDate = requestBody[key];

        if (!isValidDate(currDate)) {

            hasWrongInputs = true;
            wrongInputs += `\t- <${key}> has to be a valid date with yyyy-mm-dd format\n`
        }

    });

    if (!Array.isArray(requestBody["series_details"])) {

        hasWrongInputs = true;
        wrongInputs += `\t- <series_details> has to be a list of numbers [Empty list [] if isSeries = false]\n`
    } else {

        if(typeof requestBody["isSeries"] === 'boolean' && requestBody["isSeries"] === false && requestBody["series_details"].length != 0){

            hasWrongInputs = true;
            wrongInputs += `\t- <series_details> has to be Empty list [] if isSeries = false\n`
        }

        requestBody["series_details"].forEach((series_number) => {

            if (isNaN(series_number) || typeof series_number === 'boolean' || series_number < 0) {
                hasWrongInputs = true;
                wrongInputs += `\t- <series_details> has to be a list of numbers [Empty list [] if isSeries = false]\n`
            }
        });
    }

    if (isForEdit === false) {

        if (typeof requestBody["actors"] !== 'object') {

            hasWrongInputs = true;
            wrongInputs += `\t- <actors> has to be an json object [Empty json obj {} if there is no actor]\n`
        }
    }

    if (hasWrongInputs) {

        finalErrorString += wrongInputs + "\n";
    }


    if (hasIllegalFields || hasMissedFields || hasWrongInputs) {
        return finalErrorString;
    }

    return NO_ERRORS;

};

// validate the actors info in the case of create and edit
// the function returns the errors messages NO_ERRORS string in the case of no error
const validateCreateActor = (requestBody) => {

    let requiredKeys = ["name", "picture", "site"];

    let stringInputs = ["name"];

    let urlInputs = ["picture", "site"];

    let requestKeys = Object.keys(requestBody);

    let hasIllegalFields = false;
    let illegalFields = "* Illegal fields (Bad Request):\n";

    let hasMissedFields = false;
    let missedFields = "* Missed required fields (Bad Request):\n"

    let hasWrongInputs = false;
    let wrongInputs = "* Wrong Inputs (Bad Request):\n";

    let finalErrorString = "";

    requestKeys.forEach((key) => {

        if (!requiredKeys.includes(key)) {

            hasIllegalFields = true;

            illegalFields += `\t- <${key}>\n`;
        }
    });

    if (hasIllegalFields) {

        finalErrorString += illegalFields + "\n";
    }


    requiredKeys.forEach((key) => {

        if (!requestKeys.includes(key)) {

            hasMissedFields = true;

            missedFields += `\t- <${key}>\n`;
        }

    });

    if (hasMissedFields) {

        finalErrorString += missedFields + "\n";
    }

    stringInputs.forEach((key) => {

        let currString = requestBody[key];

        if ((typeof currString !== 'string') || (currString.length < 2)) {

            hasWrongInputs = true;
            wrongInputs += `\t- <${key}> has to be string with min length 2 chars\n`
        }

    });

    urlInputs.forEach((key) => {

        let currUrl = requestBody[key];

        if (!isValidHttpUrl(currUrl)) {

            hasWrongInputs = true;
            wrongInputs += `\t- <${key}> has to be valid HTTP URL\n`
        }

    });

    if (hasWrongInputs) {

        finalErrorString += wrongInputs + "\n";
    }

    if (hasIllegalFields || hasMissedFields || hasWrongInputs) {
        return finalErrorString;
    }

    return NO_ERRORS;

};


// eache method makes the needed validations and it returns the error message if there is an error
// if everything is ok it makes the needed action to the data
module.exports = {

    // CREATE
    createMovie: function (req, res) {

        readFile(data => {

            if (data[req.body.id]) {

                return res.status(400).send("Media ID already exists (Bad Request)\n");

            } else {

                let finalErrorString = validateeMediaInfo(req.body, false);

                if (finalErrorString !== NO_ERRORS) {

                    return res.status(400).send(finalErrorString);
                }

                data[req.body.id] = req.body;

                writeFile(JSON.stringify(data, null, 2), () => {
                    res.status(200).send('New media added');
                });
            }
        },
            true);
    },

    // UPDATE
    updateMovie: function (req, res) {

        let allowedToChangeKeys = ["name", "picture", "director", "date", "rating", "isSeries", "series_details"];

        let finalErrorString = validateeMediaInfo(req.body, true);

        if (finalErrorString !== NO_ERRORS) {

            return res.status(400).send(finalErrorString);
        }

        readFile(data => {

            const mediaId = req.params["id"];

            if (data[mediaId]) {

                let requestKeys = Object.keys(req.body);
                
                requestKeys.forEach((key) => {

                    if(allowedToChangeKeys.includes(key)){

                        data[mediaId][key] = req.body[key];
                    } else {

                        return res.status(400).send(`Not Allowed to cahnge <${key}>`);
                    }

                });

            } else {

                return res.status(400).send("Media Not Exists (Bad Request)");
            }


            writeFile(JSON.stringify(data, null, 2), () => {
                res.status(200).send(`Media id: ${mediaId} updated`);
            });
        },
            true);
    },

    addActorToMovie: function (req, res) {

        readFile(data => {

            const mediaId = req.params["id"];

            if (!data[mediaId])
                return res.status(400).send("Media Not Exists (Bad Request)");

            if (data[mediaId].actors[req.body.name]) {

                return res.status(400).send("Actor Already Exists (Bad Request)");

            } else {

                let finalErrorString = validateCreateActor(req.body);

                if (finalErrorString !== NO_ERRORS) {

                    return res.status(400).send(finalErrorString);
                }

                data[mediaId].actors[req.body.name] = req.body;

                writeFile(JSON.stringify(data, null, 2), () => {
                    res.status(200).send(`Media id: ${mediaId} updated`);
                });
            }
        },
            true);
    },

    getMovies: function (req, res) {

        readFile(data => {

            res.status(200).send(data);
        },
            true);
    },

    getMovie: function (req, res) {

        readFile(data => {

            // add the new user
            const mediaId = req.params["id"];
            if (!data[mediaId])
                return res.status(400).send("Media Not Exists (Bad Request)");

            res.status(200).send(data[mediaId]);
        },
            true);
    },
    // DELETE
    deleteActorFromMovie: function (req, res) {

        readFile(data => {

            // add the new user
            const mediaId = req.params["id"];

            if (!data[mediaId])
                return res.status(400).send("Media Not Exists (Bad Request)");

            const actorName = req.params["actorName"];

            if (!(data[mediaId].actors[actorName]))
                return res.status(400).send("Actor Not Exists (Bad Request)");

            delete data[mediaId].actors[actorName];

            writeFile(JSON.stringify(data, null, 2), () => {
                res.status(200).send(`Media id: ${mediaId} updated`);
            });
        },
            true);
    },

    deleteMovie: function (req, res) {

        readFile(data => {

            // add the new user
            const mediaId = req.params["id"];

            if (!data[mediaId])
                return res.status(400).send("Media Not Exists (Bad Request)");

            delete data[mediaId];

            writeFile(JSON.stringify(data, null, 2), () => {
                res.status(200).send(`Media id: ${mediaId} updated`);
            });
        },
            true);
    }
};

function isValidHttpUrl(string) {
    let url;

    try {
        url = new URL(string);
    } catch (_) {
        return false;
    }

    return url.protocol === "http:" || url.protocol === "https:";
}

function isValidDate(dateString) {
    // First check for the pattern
    var regex_date = /^\d{4}\-\d{1,2}\-\d{1,2}$/;

    if (!regex_date.test(dateString)) {
        return false;
    }

    // Parse the date parts to integers
    var parts = dateString.split("-");
    var day = parseInt(parts[2], 10);
    var month = parseInt(parts[1], 10);
    var year = parseInt(parts[0], 10);

    // Check the ranges of month and year
    if (year < 1000 || year > 3000 || month == 0 || month > 12) {
        return false;
    }

    var monthLength = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];

    // Adjust for leap years
    if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0)) {
        monthLength[1] = 29;
    }

    // Check the range of the day
    return day > 0 && day <= monthLength[month - 1];
}