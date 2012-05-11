jQuery('document').ready(function(){
  jQuery('div.form-wrapper').delegate('.unlimitedfield_remove_button', 'click', function(){
    // get parent table row
    var row = jQuery(this).closest('td').parent('tr');

    // hide and empty values
    jQuery(row).hide();
    jQuery(row).find('input').val('');
    jQuery(row).find('select').val('');
    jQuery(row).find('textarea').text('');
    jQuery(row).find('checkbox').removeAttr('checked');
    jQuery(row).find('options').removeAttr('selected');
  
    // fix table row classes
    var table_id = jQuery(row).parent('tbody').parent('table').attr('id');
    jQuery('#'+table_id+' tr.draggable:visible').each(function(index, element){
      jQuery(element).removeClass('odd').removeClass('even');
      if((index%2) == 0){
        jQuery(element).addClass('odd');
      } else {
        jQuery(element).addClass('even');
      }
    });
  });
});