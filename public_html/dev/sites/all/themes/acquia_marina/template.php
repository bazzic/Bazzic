<?php

function acquia_marina_views_pre_render(&$view) {
  if ($view->name == 'home_page' || $view->name == 'open_studios_calendar') {
	foreach ($view->result as $index => $result) {
	  if (empty($result->field_field_link)) {
	    $link = drupal_get_path_alias('node/' . $result->nid);
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