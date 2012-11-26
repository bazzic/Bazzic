<?php

/**
 * Preprocess variables for html.tpl.php
 *
 * @see system_elements()
 * @see html.tpl.php
 */
function commerce_kickstart_theme_preprocess_html(&$variables) {
  // Add conditional stylesheets for IE
  drupal_add_css(path_to_theme() . '/css/commerce-kickstart-theme-ie-lte-8.css', array('group' => CSS_THEME, 'weight' => 23, 'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE), 'preprocess' => FALSE));
  drupal_add_css(path_to_theme() . '/css/commerce-kickstart-theme-ie-lte-7.css', array('group' => CSS_THEME, 'weight' => 24, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'preprocess' => FALSE));

  // Add external libraries.
  drupal_add_library('commerce_kickstart_theme', 'selectnav');
}

/**
 * Implements hook_library().
 */
function commerce_kickstart_theme_library() {
  $libraries['selectnav'] = array(
    'title' => 'Selectnav',
    'version' => '',
    'js' => array(
      libraries_get_path('selectnav.js') . '/selectnav.min.js' => array(),
    ),
  );
  return $libraries;
}

/**
 * Preprocess field.
 */
function commerce_kickstart_theme_preprocess_field(&$variables) {
  $element = $variables['element'];
  if ($element['#entity_type'] != 'node' || $element['#field_name'] != 'title_field') {
    return;
  }

  $variables['theme_hook_suggestions'][] = 'field__fences_h2__node';
}

function commerce_kickstart_preprocess_page(&$vars) {
  if (isset($vars['node']->type)) {
    $vars['theme_hook_suggestions'][] = 'page__' . $vars['node']->type;
  }
}

function commerce_kickstart_theme_media_gallery_item($variables) {
  $file_parts = explode('/', $variables['link_path']);
  $fid = $file_parts[3];
  
  $image = $variables['image'];
  $link_path = $variables['link_path'];
  $attributes = array();
  if (!empty($variables['classes'])) {
    $attributes['class'] = $variables['classes'];
  }
  if (!empty($variables['title'])) {
    // I'm fairly sure I don't like this solution.  But as Alex mentions in
    // theme_media_gallery_file_field_inline() the File Styles module isn't allowing
    // us access to the render array pre-rendering, so I'm doing a str_replace()
    // here specifically to address the title and alt for thumbnails.  This had
    // to be further modified to remove and then add the title and alt attributes
    // video thumbnails had no title and alt attributes so the string replace was
    // not triggering for them.
    $new_image = str_replace(array('title=""', 'alt=""'), array('', ''), $image);
    $image = str_replace('/>', ' title="'.$variables['title'].'" alt="'.$variables['title'].'" />', $new_image);;
  }

  // Add sliding door top div and wrappers
  $item = '<div class="media-gallery-item"><div class="top"><div class="top-inset-1"><div class="top-inset-2"></div></div></div><div class="gallery-thumb-outer"><div class="gallery-thumb-inner">';
  // Create a link around the image
  $item .= empty($variables['no_link']) ? l($image, $link_path, array('html' => TRUE, 'attributes' => $attributes)) : $image;
  $item .= render(drupal_get_form('photobook_gallery_size_form', $fid));
  // Add sliding door bottom div and close wrappers
  $item .= '</div></div><div class="bottom"><div class="bottom-inset-1"><div class="bottom-inset-2"></div></div></div></div>';
  return $item;
}
