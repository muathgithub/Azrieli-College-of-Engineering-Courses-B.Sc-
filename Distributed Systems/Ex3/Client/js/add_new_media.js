// thsi variable used for the generating of series number fileds if the media is a serie
let temp_season_num = 1;

// the i jquery method executes when the document is ready
$(document).ready(function () {

  $('.Series_info').css("display", "none");

  // Series_info" style="display: none;"
  $("form[name='Media_form']").validate({
    // Specify validation rules
    rules: {
      "name": {
        minlength: 2
      },
      "id_field": {
        minlength: 2
      },
      "rating": {

        digits: true,
        min: 1,
        max: 5
      },
      "director": {
        minlength: 2
      },
      "episode": {
        required: true
      }


    },
    // Specify validation error messages
    messages: {

      id_field: {
        digits: "Please enter only digits",

      },
      rating: {
        digits: "Please enter only digits",
        min: "rating must between 1 to 5",
        max: "rating must between 1 to 5"
      },

    }
  });

  $('#isSeries').val(false);

  $('#isSeries').change(function () {

    $('.Series_info').toggle();
    $('#isSeries').val(this.checked);
  });

  // modifing the number of epsodes fileds according to the user chose
  $('#season').change(function () {
    let season_num = parseInt($(this).val());
    if (season_num <= 0 || season_num === temp_season_num) {
      return;
    }
    if (season_num < temp_season_num) {
      for (let i = 0; i < temp_season_num - season_num; i++) {
        $('.Episode-group').children().last().remove();
      }
    } else {

      $('.Episode-group').append(generate_episodes_cards(season_num - temp_season_num));

    }


    temp_season_num = season_num;

  });


  $('#Media_form').submit(function (event) {
    if (!$("#Media_form").valid()) return;

    // sending the data to the server
    $.ajax({
      type: 'POST', // define the type of HTTP verb we want to use (POST for our form)
      url: 'http://localhost:3001/medias', // the url where we want to POST
      contentType: 'application/json',
      data: JSON.stringify({
        name: $("#name").val(),
        id: $("#id_field").val(),
        date: $("#date").val(),
        director: $("#director").val(),
        rating: parseInt($("#rating").val()),
        picture: $("#picture").val(),
        isSeries: ($('#isSeries').val() === 'true'),
        series_details: ($('#isSeries').val() === 'true') ? get_series_details() : [],
        actors: []
      }),
      processData: false,
      // // dataType: 'json', // what type of data do we expect back from the server
      encode: true,
      success: function (data, textStatus, jQxhr) {

        location.href = "/list";
      },
      error: function (jqXhr, textStatus, errorThrown) {
        showErrorMessage(jqXhr.responseText);
        console.log(jqXhr.responseText);
      }
    })

    // stop the form from submitting the normal way and refreshing the page
    event.preventDefault();
  });

  $(".error_message_button").click(function () {
    $(".error_full_cover").css('display', 'none');
  });

});

// This function generates the episods fileds to add them to the html page
// the function returns the html of the episods filed cards
function generate_episodes_cards(Cards_num) {

  let episodesCardsHTML = "";

  if (Cards_num < 1) {
    Cards_num = 1;
  }

  let size = (parseInt(Cards_num) + parseInt(temp_season_num));

  for (let j = parseInt(temp_season_num); j < size; j++) {

    episodesCardsHTML +=
      ` <div id="episode-series-group" class="form-group">
        <label for="episode">Season ${j + 1}  episode num:</label>
        <input type="number" min='1' value = 1 class="form-control" name="episode_${j}" id="episode_${j}" placeholder="Type episode"
            required>
        
    </div>`;
  }

  return episodesCardsHTML;
}

// this function reads the seriess info from the fileds and returns them as a list
function get_series_details() {

  let series_details = [];

  for (let i = 0; i < temp_season_num; i++) {

    series_details.push(parseInt($(`#episode_${i}`).val()));
  }

  return series_details;
}

// This function shows an error message in case of error
function showErrorMessage(errorMessage) {

  $(".error_message_span").text(errorMessage);
  $(".error_full_cover").css('display', 'block');
}