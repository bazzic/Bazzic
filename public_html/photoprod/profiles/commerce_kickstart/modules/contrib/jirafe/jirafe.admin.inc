<?php

/**
 * @file
 * Configuration for Jirafe module.
 */

/**
 * Displays Jirafe configuration.
 */
function jirafe_dashboard_admin_page($form, $form_state) {
  // TODO: Add API application and user managing.
  $form = array();

  $params = array(
    'app_id' => '',
    'token' => '',
    'sites' => array(),
    'users' => array(),
  );

  $config = variable_get('jirafe_config', $params);

  $form['app_info'] = array(
    '#title' => t('Application information'),
    '#type' => 'fieldset',
  );
  $form['app_info']['app_id'] = array(
    '#title' => t('Application ID'),
    '#type' => 'textfield',
    '#default_value' => $config['app_id'],
  );
  $form['app_info']['token'] = array(
    '#title' => t('Application token'),
    '#type' => 'textfield',
    '#default_value' => $config['token'],
  );

  if (!empty($config['sites'])) {
    $form['site_info'] = array(
      '#title' => t('Site information'),
      '#type' => 'fieldset',
    );
    foreach($config['sites'] as $site) {
      $form['site_info'][$site['site_id']] = array(
        '#title' => $site['description'],
        '#type' => 'fieldset',
      );
      $form['site_info'][$site['site_id']]['site_id'] = array(
        '#title' => t('Site ID'),
        '#type' => 'textfield',
        '#default_value' => $site['site_id'],
        '#attributes' => array('readonly' => 'readonly'),
      );
      $form['site_info'][$site['site_id']]['site_url'] = array(
        '#title' => t('Site URL'),
        '#type' => 'textfield',
        '#default_value' => $site['url'],
        '#attributes' => array('readonly' => 'readonly'),
      );
    }
  }

  if (!empty($config['users'])) {
    $form['user_info'] = array(
      '#title' => t('User information'),
      '#type' => 'fieldset',
    );
    foreach($config['users'] as $index => $user) {
      $form['user_info']['user_' . $index] = array(
        '#title' => "{$user['first_name']} {$user['last_name']}",
        '#type' => 'fieldset',
      );
      $form['user_info']['user_' . $index]['user_mail'] = array(
        '#title' => t('User mail'),
        '#type' => 'textfield',
        '#default_value' => $user['email'],
        '#attributes' => array('readonly' => 'readonly'),
      );
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit callback for the Jirafa settings form.
 *
 * Overwrite the Jirafe token and application ID.
 */
function jirafe_dashboard_admin_page_submit($form, $form_state) {
  $config = variable_get('jirafe_config');
  $config['token'] = trim($form_state['values']['token']);
  $config['app_id'] = trim($form_state['values']['app_id']);
  variable_set('jirafe_config', $config);
}
