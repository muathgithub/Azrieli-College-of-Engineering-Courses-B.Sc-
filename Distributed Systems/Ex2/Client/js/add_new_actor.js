// thsi variable used for the generating of series number fileds if the media is a serie
let temp_season_num = 1;

// the i jquery method executes when the document is ready
$(document).ready(function () {

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

  $('#Actors_form').submit(function (event) {
    if (!$("#Actors_form").valid()) return;

    // Sending the data to the server
    $.ajax({
      type: 'PUT', // define the type of HTTP verb we want to use (POST for our form)
      url: `http://localhost:3001/medias/${media_id}/actors`, // the url where we want to POST
      contentType: 'application/json',
      data: JSON.stringify({
        name: $("#name").val(),
        picture: $("#picture").val(),
        site: $("#site").val(),
      }),
      processData: false,
      // dataType: 'json', // what type of data do we expect back from the server
      encode: true,
      success: function (data, textStatus, jQxhr) {
        console.log(data);
        location.href = "/";

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
