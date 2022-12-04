let media_id;
let actor_name;

// this jquery method executes when the document is ready
$(document).ready(function () {

  let url_parts = location.href.split("/");
  let media_id = url_parts[url_parts.length - 2];

  let Actors_object_list = [];

  // ajax call for getting the date from the server
  $.ajax({

    type: 'GET',
    url: `http://localhost:3001/medias/${media_id}`,
    success: function (data, status, xhr) {

      for (let key in data.actors) {
        Actors_object_list.push(data.actors[key]);
      }

      console.log(Actors_object_list);

      $(".All_Actors_flex").html(generateActorsCards(Actors_object_list));

      connect_delete_buttons(Actors_object_list);

    },

    processData: false,
    dataType: 'json', // what type of data do we expect back from the server
    encode: true,

    // dealing with the cases of erorr of the ajax calls
    error: function (jqXhr, status, error) {

      showErrorMessage(jqXhr.responseText);
        console.log(jqXhr.responseText);
    }
  });

  $(".error_message_button").click(function () {
    $(".error_full_cover").css('display', 'none');
  });

  $(".Alert_message_no").click(function () {

    $(".Alert_full_cover").css('display', 'none');
  });

  $(".Alert_message_yes").click(function () {

    $.ajax({
      type: 'PUT', // define the type of HTTP verb we want to use (POST for our form)
      url: `http://localhost:3001/medias/${media_id}/actors/${actor_name}`, // the url where we want to POST
      success: function (data, textStatus, jQxhr) {

        location.href = `/${media_id}/actors_list`;
      },
      error: function (jqXhr, textStatus, errorThrown) {

        showErrorMessage(jqXhr.responseText);
        console.log(jqXhr.responseText);
      }
    })

  });

});

// This function connects the delte buttons to there actions according to the buttons type
// it returns nothing (no value returned)
function connect_delete_buttons(Actors_object_list) {

  $('.Delet_button').click(function () {
    // check if any button clicked


    let idParts = $(this).attr('id').split('_');

    actor_card_id = idParts[idParts.length - 1];
    
    actor_name = Actors_object_list[parseInt(actor_card_id)].name;

    $(".Alert_full_cover").css('display', 'block');
  });
}

// This function generates the actors cards to add them to the html page
// the function returns the html of the gemerated cards
function generateActorsCards(Actors_object_list) {

  let actorsCardsHTML = "";

  Actors_object_list.forEach((element, i) => {
    actorsCardsHTML +=
      `<div class="Actor_flex">
      <div class="Image_flex">
        <img class="Actor_image" src="${Actors_object_list[i].picture}" />
      </div>
  
      <div class="Actor_info_flex">
  
        <span class="Actor_name"> ${Actors_object_list[i].name} </span>
        <a class="Actor_other_info" target="_blank" href="${Actors_object_list[i].site}">Fans Website</a>

  
      </div>
  
      <div class="Buttons_flex">
  
        <input class="Buttons_style Delet_button" type="button" id="Delet_${i}" value="Delet">
  
      </div>
    </div>`;
  });

  return actorsCardsHTML;
}

// This function shows an error message in case of error
function showErrorMessage(errorMessage) {

  $(".error_message_span").text(errorMessage);
  $(".error_full_cover").css('display', 'block');
}