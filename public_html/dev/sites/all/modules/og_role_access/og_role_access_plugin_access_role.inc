<?php

/**
 * @file
 * Views access plugin that provides og-role-type-based access control.
 */

class og_role_access_plugin_access_role extends views_plugin_access {
  function get_access_callback() {
    return array('og_role_access_check_role', array($this->options['og_roles']));
  }

  function summary_title() {
    $count = count($this->options['og_roles']);
    if ($count < 1) {
      return t('No OG role(s) selected');
    }
    elseif ($count > 1) {
      return t('Multiple OG roles');
    }
    else {
      $ids = og_get_global_roles();
      $id = reset($this->options['og_roles']);
      return check_plain($ids[$id]);
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['og_roles'] = array('default' => array());
    
    return $options;
  }
  
  function options_form(&$form, &$form_state) {
    $og_roles = og_get_global_roles();
    $form['og_roles'] = array(
      '#type' => 'checkboxes',
      '#options' => array_map('check_plain', $og_roles),
      '#title' => t('OG Roles'),
      '#default_value' => $this->options['og_roles'],
      '#description' => t('Only users with this role can display the view as tab.'),
    );
  }

  function options_validate(&$form, &$form_state) {
    if (!array_filter($form_state['values']['access_options']['og_roles'])) {
      form_error($form['og_roles'], t('You must select at least one OG role'));
    }
  }

  function options_submit(&$form, &$form_state) {
    $form_state['values']['access_options']['og_roles'] = array_filter($form_state['values']['access_options']['og_roles']);
  }
}