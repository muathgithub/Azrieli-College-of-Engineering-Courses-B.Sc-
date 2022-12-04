// thsi variable used for the generating of series number fileds if the media is a serie
let temp_season_num = 1;

// the i jquery method executes when the document is ready
$(document).ready(function () {

  let hasNoActors = false;

  $('.actorManuallyInfo').css("display", "none");

  let url_parts = location.href.split("/");
  let media_id = url_parts[url_parts.length - 2];

  // Series_info" style="display: none;"
  $("form[name='Actors_form']").validate({
    // Specify validation rules
    rules: {
      "name": {
        minlength: 2
      }
    },

  });

  $('#isManuallyDetailsFill').val(false);

  $('#isManuallyDetailsFill').change(function () {

    $('.actorManuallyInfo').toggle();
    $("#actors_names_options").prop("disabled", ((($('#isManuallyDetailsFill').val() === 'false') || hasNoActors)? true : false));
    $('#isManuallyDetailsFill').val(this.checked);
  });

  // Sending the data to the server
  $.ajax({
    type: 'GET', // define the type of HTTP verb we want to use (POST for our form)
    url: `http://localhost:3001/actors`, // the url where we want to POST
    contentType: 'application/json',
    processData: false,
    // dataType: 'json', // what type of data do we expect back from the server
    encode: true,
    success: function (actorsJSONObjectsArray, textStatus, jQxhr) {

      if (actorsJSONObjectsArray.length === 0) {

        hasNoActors = true;
        $("#actors_names_options").prop("disabled", true);
        $("#actors_names_options").html(`<option>The List Is Empty</option>\n`);
      } else {

        let htmlOptionsString = "";

        actorsJSONObjectsArray.forEach(currActorJSON => {

          htmlOptionsString += `<option>${currActorJSON._id}</option>\n`;
        });

        $("#actors_names_options").html(htmlOptionsString);
      }



    },
    error: function (jqXhr, textStatus, errorThrown) {
      showErrorMessage(jqXhr.responseText);
      console.log(jqXhr.responseText);
    }
  })

  $('#Actors_form').submit(function (event) {
    if (!$("#Actors_form").valid()) return;

    let actorDataJSON;

    if ($('#isManuallyDetailsFill').val() === 'true') {
      actorDataJSON = {
        name: $("#name").val(),
        picture: $("#picture").val(),
        site: $("#site").val(),
      };
    } else {
      actorDataJSON = {
        name: $("#actors_names_options").children("option:selected").val(),
      };
    }

    // Sending the data to the server
    $.ajax({
      type: 'PUT', // define the type of HTTP verb we want to use (POST for our form)
      url: `http://localhost:3001/medias/${media_id}/actors`, // the url where we want to POST
      contentType: 'application/json',
      data: JSON.stringify(actorDataJSON),
      processData: false,
      // dataType: 'json', // what type of data do we expect back from the server
      encode: true,
      success: function (data, textStatus, jQxhr) {
        console.log(data);
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

// This function shows an error message in case of error
function showErrorMessage(errorMessage) {

  $(".error_message_span").text(errorMessage);
  $(".error_full_cover").css('display', 'block');
}
