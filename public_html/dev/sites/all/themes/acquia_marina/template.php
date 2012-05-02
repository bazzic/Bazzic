<?php

function acquia_marina_views_pre_render(&$view) {
  if ($view->name == 'home_page' || $view->name == 'open_studios_calendar') {
  	global $base_url;
	foreach ($view->result as $index => $result) {
	  if (empty($result->field_field_link)) {
	    $link = $base_url . '/' . drupal_get_path_alias('node/' . $result->nid);
	    $view->result[$index]->field_field_link[0]['rendered'] = array(
	      '#markup' => $link,
	      '#access' => TRUE,
	    );
	    $view->result[$index]->field_field_link[0]['raw'] = array(
	      'value' => $link,
	      'format' => NULL,
	      'safe_value' => $link,
	    );
      }
    }
  }
}

function acquia_marina_menu_link(array $variables) {
  // This allows us to highlight the Home menu item.	
  if ($variables['element']['#title'] == 'Home' && $_GET['q'] == 'home-page') {
    $variables['element']['#localized_options']['attributes']['class'][] = 'active-trail';
    $variables['element']['#attributes']['class'][] = 'active-trail';
  }
  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}