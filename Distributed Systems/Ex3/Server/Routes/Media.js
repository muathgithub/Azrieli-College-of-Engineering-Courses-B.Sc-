const { Console } = require('console');
const fs = require('fs');
const path = require('path');
const Actor = require('../Models/ActorSchema');
const Media = require('../Models/MediaSchema')

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

        if (typeof requestBody["isSeries"] === 'boolean' && requestBody["isSeries"] === false && requestBody["series_details"].length != 0) {

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
    createMovie: async function (req, res) {

        let reqBody = req.body;

        let isMediaExists = await Media.findOne({ _id: reqBody.id }).select("_id").lean();

        if (isMediaExists) {

            return res.status(400).send("Media ID already exists (Bad Request)\n");

        } else {

            let finalErrorString = validateeMediaInfo(req.body, false);

            if (finalErrorString !== NO_ERRORS) {

                return res.status(400).send(finalErrorString);
            }

            reqBody._id = reqBody.id;
            delete reqBody.id;

            const media = new Media(reqBody);

            media.save().then((media) =>
                res.status(201).send(media)
            ).catch(e => res.status(400).send(e))

        }

    },

    // UPDATE
    updateMovie: async function (req, res) {

        let reqBody = req.body;

        const mediaId = req.params["id"];

        let allowedToChangeKeys = ["name", "picture", "director", "date", "rating", "isSeries", "series_details"];

        let finalErrorString = validateeMediaInfo(reqBody, true);

        if (finalErrorString !== NO_ERRORS) {

            return res.status(400).send(finalErrorString);
        }

        let isMediaExists = await Media.findOne({ _id: mediaId }).select("_id").lean();

        if (isMediaExists) {

            let requestKeys = Object.keys(reqBody);

            let mediaData = Media.findOne({ _id: mediaId });

            requestKeys.forEach((key) => {

                if (allowedToChangeKeys.includes(key)) {

                    mediaData[key] = reqBody[key];
                } else {

                    return res.status(400).send(`Not Allowed to cahnge <${key}>`);
                }

            });

        } else {

            return res.status(400).send("Media Not Exists (Bad Request)");
        }

        reqBody._id = mediaId;

        Media.findByIdAndUpdate(mediaId, reqBody, { new: true, runValidators: true }).then(media => {
            if (!media) {
                return res.status(404).send()
            }
            else {
                console.log(media);
                res.status(200).send(media);
            }
        }).catch(e => res.status(400).send(e))
    },

    addActorToMovie: async function (req, res) {

        let reqBody = req.body;

        const mediaId = req.params["id"];

        let isMediaExists = await Media.findOne({ _id: mediaId }).select("_id").lean();

        if (!isMediaExists)
            return res.status(400).send("Media Not Exists (Bad Request)");


        Media.findOne({ _id: mediaId }).lean().exec(async function (err, mediaData) {

            console.log(reqBody);

            if (mediaData.actors.includes(reqBody.name)) {

                return res.status(400).send("Actor Already Exists (Bad Request)");

            } else {

                const actorName = reqBody.name;

                let isActorExists = await Actor.findOne({ _id: actorName }).select("_id").lean();

                if (isActorExists) {

                    mediaData.actors.push(reqBody.name);

                    Media.findByIdAndUpdate(mediaData._id, mediaData, { new: true, runValidators: true }).then(media => {
                        if (!media) {
                            return res.status(404).send()
                        }
                        else {
                            console.log(media);
                            res.status(200).send(media);
                        }
                    }).catch(e => res.status(400).send(e))

                } else {

                    let finalErrorString = validateCreateActor(reqBody);

                    if (finalErrorString !== NO_ERRORS) {

                        return res.status(400).send(finalErrorString);
                    }

                    reqBody._id = reqBody.name;

                    const actor = new Actor(reqBody);

                    actor.save().then((actor) => {

                        mediaData.actors.push(reqBody.name);

                        Media.findByIdAndUpdate(mediaData._id, mediaData, { new: true, runValidators: true }).then(media => {
                            if (!media) {
                                return res.status(404).send()
                            }
                            else {
                                console.log(media);
                                res.status(200).send(media);
                            }
                        }).catch(e => res.status(400).send(e))
                    }

                    ).catch(e => res.status(400).send(e));

                }

            }
        });
    },

    addActorToList: async function (req, res) {

        let reqBody = req.body;

        const actorName = reqBody.name;
        
        let isActorExists = await Actor.findOne({ _id: actorName }).select("_id").lean();
        
        if (isActorExists) {
            
            return res.status(400).send("Actor Already Exists (Bad Request)");

        } else {
            

            let finalErrorString = validateCreateActor(reqBody);

            if (finalErrorString !== NO_ERRORS) {

                return res.status(400).send(finalErrorString);
            }

            reqBody._id = reqBody.name;

            const actor = new Actor(reqBody);

            actor.save().then(res.status(201).send(actor)).catch(e => res.status(400).send(e));

        }

    },



    getMovies: function (req, res) {

        Media.find().lean().exec(function (err, mediasData) {

            mediasData.forEach((currMediaJSON) => {

                currMediaJSON.id = currMediaJSON._id;
                delete currMediaJSON._id;
            });

            return res.status(200).send(mediasData);
        })
    },

    getMovie: async function (req, res) {

        const mediaId = req.params["id"];

        let isMediaExists = await Media.findOne({ _id: mediaId }).select("_id").lean();

        if (!isMediaExists) {

            return res.status(400).send("Media Not Exists (Bad Request)");
        } else {

            Media.findOne({ _id: mediaId }).lean().exec(async function (err, mediaData) {

                const actorsJSONArray = await Actor.find().where('_id').in(mediaData.actors).exec();

                console.log(actorsJSONArray);

                mediaData.actors = actorsJSONArray;

                return res.status(200).send(mediaData);
            });
        }

    },
    // DELETE
    deleteActorFromMovie: async function (req, res) {

        const mediaId = req.params["id"];
        const actorName = req.params.actorName;

        let isMediaExists = await Media.findOne({ _id: mediaId }).select("_id").lean();

        if (!isMediaExists)
            return res.status(400).send("Media Not Exists (Bad Request)");


        Media.findOne({ _id: mediaId }).lean().exec(async function (err, mediaData) {

            if (!mediaData.actors.includes(actorName)) {

                return res.status(400).send("Actor Not Exists (Bad Request)");

            } else {

                mediaData.actors.splice(mediaData.actors.indexOf(actorName), 1);

                Media.findByIdAndUpdate(mediaData._id, mediaData, { new: true, runValidators: true }).then(media => {
                    if (!media) {
                        return res.status(404).send()
                    }
                    else {
                        console.log(media);
                        res.status(200).send(`Media id: ${mediaId} updated`);
                    }
                }).catch(e => res.status(400).send(e))
            }


        });

    },

    deleteMovie: async function (req, res) {


        const mediaId = req.params["id"];

        let isMediaExists = await Media.findOne({ _id: mediaId }).select("_id").lean();

        if (!isMediaExists)
            return res.status(400).send("Media Not Exists (Bad Request)");



        await Media.deleteOne({ _id: mediaId });

        return res.status(200).send(`Media id: ${mediaId} updated`);

    },

    getActors: function (req, res) {

        Actor.find().lean().exec(function (err, actorsData) {
            return res.status(200).send(actorsData);
        });
    },
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