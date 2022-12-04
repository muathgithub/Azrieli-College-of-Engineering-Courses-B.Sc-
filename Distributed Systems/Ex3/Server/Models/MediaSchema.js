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

var mediaSchema = new mongoose.Schema({
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

    date: {
        type: String,
        required: true,
        trim: true,
        validate(dateString) {

            if (!isValidDate(dateString)) {
                throw new Error('Invalid Date');
            }
        }
    },
    director: {
        type: String,
        required: true,
        trim: true,
        minLength: 2
    },
    rating: {
        type: Number,
        required: true,
        min: 1,
        max: 5
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

    isSeries: {
        type: Boolean,
        required: false,
        default: false,
    },

    series_details: {
        type: Array,
        required: false,
        default: [],
    },

    actors: {
        type: Array,
        required: false,
        default: [], 
    }

}, { timestamps: true });
mediaSchema.plugin(id_validator);
// mediaSchema.index("completed");


const Media = mongoose.model('Media', mediaSchema );

module.exports = Media