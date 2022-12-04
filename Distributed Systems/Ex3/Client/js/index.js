let media_id;

// the jquery method executes when the document is ready
$(document).ready(function () {

  let media_object_list = [];
  let sort_by = "Date";
  let sort_order = "Descending";
  let sorted_data;

  // ajax call for getting the media date from the server
  $.ajax({

    type: 'GET',
    url: "http://localhost:3001/medias",
    success: function (data, status, xhr) {

      for (let key in data) {
        media_object_list.push(data[key]);
      }

      // sorting the data as asked in the exercise by release date and descending order
      sorted_data = Sort(media_object_list, sort_by, sort_order);

      // displaying the date on the web page
      $(".All_Media_flex").html(generateMediaCards(sorted_data));

      // connecting the buttons to there on click actions
      connect_buttons_onClick(sorted_data);

    },

    processData: false,
    dataType: 'json', // what type of data do we expect back from the server
    encode: true,

    // dealing with the cases of erorr of the ajax calls
    error: function (jqXhr, status, error) {

      // printing the error to the console and displaying an alert message
      showErrorMessage(jqXhr.responseText);
      console.log(jqXhr.responseText);
    }
  });

  $(".error_message_button").click(function () {
    $(".error_full_cover").css('display', 'none');
  });

  $("select.Sort_by").change(function () {
    // read the sort by and sort order to sort the data again

    sort_by = $(this).children("option:selected").val();
    sorted_data = Sort(media_object_list, sort_by, sort_order);

    $(".All_Media_flex").html(generateMediaCards(sorted_data));
    connect_buttons_onClick(sorted_data);
  });

  $("select.Sort_order").change(function () {
    // read the sort by and sort order to sort the data again

    sort_order = $(this).children("option:selected").val();
    sorted_data = Sort(media_object_list, sort_by, sort_order);
    $(".All_Media_flex").html(generateMediaCards(sorted_data));
    connect_buttons_onClick(sorted_data);
  });

  // connecting the controls buttons to there functions and to the proper location.hrefs
  $(".Add_media_button").click(function () {

    location.href = "/add_new_media";

  });
  $(".Add_actor_button").click(function () {

    location.href = "/add_new_actor_to_list";

  });

  $(".Alert_message_no").click(function () {

    $(".Alert_full_cover").css('display', 'none');
  });

  // deleting the media in case of pressing the yes button in the alert message
  $(".Alert_message_yes").click(function () {

    $.ajax({
      type: 'DELETE', // define the type of HTTP verb we want to use (POST for our form)
      url: `http://localhost:3001/medias/${media_id}`, // the url where we want to POST
      success: function (data, textStatus, jQxhr) {

        location.href = "/list";
      },
      error: function (jqXhr, textStatus, errorThrown) {

        showErrorMessage(jqXhr.responseText);
        console.log(jqXhr.responseText);
      }
    })
  });

});

// This function connects the buttons to there actions according to the buttons type
// it returns nothing (no value returned)
function connect_buttons_onClick(sorted_data) {
  $(".media_buttons").click(function (button) {

    let id_parts_list = button.target.id.split("_");
    let button_type = id_parts_list[0];
    let media_index = id_parts_list[1];

    media_id = sorted_data[parseInt(media_index)].id;

    switch (button_type) {

      case "Edit":
        location.href = `/edit_media/${media_id}`;
        break;

      case "Add":
        location.href = `/${media_id}/add_new_actor`;
        break;

      case "List":
        location.href = `/${media_id}/actors_list`;
        break;

      case "Delete":
        $(".Alert_full_cover").css('display', 'block');
        break;

    }

  });
}

// This function generates the media cards to add them to the html page
// the function returns the html og the gemerated cards
function generateMediaCards(sorted_data) {

  let mediaCardsHTML = "";

  sorted_data.forEach((element, i) => {
    mediaCardsHTML +=
      `<div class="Media_flex">
    <div class="Image_flex">
      <img class="Media_image" src="${sorted_data[i].picture}" />
    </div>

    <div class="Media_info_flex">

      <span class="Media_name"> ${sorted_data[i].name} </span>
      <span class="Media_other_info"> ID: ${sorted_data[i].id} </span>
      <span class="Media_other_info"> Release Date: ${sorted_data[i].date} </span>
      <span class="Media_other_info"> Rating:  ${sorted_data[i].rating} </span>

    </div>

    <div class="Buttons_flex">

      <input class="Buttons_style Other_button media_buttons" type="button" id="Edit_${i}" value="Edit">
      <input class="Buttons_style Other_button media_buttons" type="button" id="Add_${i}" value="Add Actor">
      <input class="Buttons_style Other_button media_buttons" type="button" id="List_${i}" value="Actors List">
      <input class="Buttons_style Delet_button media_buttons" type="button" id="Delete_${i}" value="Delete">

    </div>
  </div>`;
  });

  return mediaCardsHTML;
}

// This function sorts the media data acoording to the sort_by and sort_order inputs
// the function returns the sorted data
function Sort(media_object_list, sort_by, sort_order) {
  // sort the data array by date
  switch (sort_by) {
    case "Date":
      media_object_list.sort(date_compare);
      break;
    case "Name":
      media_object_list.sort(name_compare);
      break;

    case "Rating":
      media_object_list.sort(rating_compare);
      break;
  }

  if (sort_order === "Descending") {
    media_object_list.reverse();
  }

  return media_object_list;
}

// This function compares two dates for the use in sort method
// the funtion returns -1, 0, 1
function date_compare(media_one, media_two) {
  let media_one_date_list = media_one.date.split("-");

  let media_two_date_list = media_two.date.split("-");

  let media_one_date_obj = new Date(
    parseInt(media_one_date_list[0]),
    parseInt(media_one_date_list[1]) - 1,
    parseInt(media_one_date_list[2])
  );
  let media_two_date_obj = new Date(
    parseInt(media_two_date_list[0]),
    parseInt(media_two_date_list[1]) - 1,
    parseInt(media_two_date_list[2])
  );

  return media_one_date_obj - media_two_date_obj;
}

// This function compares two names for the use in sort method
// the funtion returns -1, 0, 1
function name_compare(media_one, media_two) {
  let media_one_name_list = media_one.name;

  let media_two_name_list = media_two.name;

  return media_one_name_list.localeCompare(media_two_name_list);
}

// This function compares two rating for the use in sort method
// the funtion returns -1, 0, 1
function rating_compare(media_one, media_two) {
  let media_one_rating_list = media_one.rating;

  let media_two_rating_list = media_two.rating;

  return media_one_rating_list - media_two_rating_list;
}

// This function shows an error message in case of error
function showErrorMessage(errorMessage) {

  $(".error_message_span").text(errorMessage);
  $(".error_full_cover").css('display', 'block');
}

