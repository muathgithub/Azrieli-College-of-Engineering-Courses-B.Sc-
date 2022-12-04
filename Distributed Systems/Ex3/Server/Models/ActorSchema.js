const mongoose = require('mongoose');
const id_validator = require ('mongoose-id-validator');

function isValidHttpUrl(string) {
    let url;

    try {
        url = new URL(string);
    } catch (_) {
        return false;
    }

    return url.protocol === "http:" || url.protocol === "https:";
}

var actorSchema = new mongoose.Schema({
    _id:{
        type: String,
        required: true,
        trim: true,
        minLength: 2
    },
    name: {
        type: String,
        required: true,
        trim: true,
        minLength: 2
    },

    picture: {
        type: String,
        required: true,
        trim: true,
        validate(pictureURL) {

            if (!isValidHttpUrl(pictureURL)) {
                throw new Error('Invalid Picture URL');
            }
        }
    },

    site: {
        type: String,
        required: true,
        trim: true,
        validate(siteURL) {

            if (!isValidHttpUrl(siteURL)) {
                throw new Error('Invalid Site URL');
            }
        }
    },

}, { timestamps: true });
actorSchema.plugin(id_validator);
// mediaSchema.index("completed");


const Actor = mongoose.model('Actor', actorSchema );

module.exports = Actor