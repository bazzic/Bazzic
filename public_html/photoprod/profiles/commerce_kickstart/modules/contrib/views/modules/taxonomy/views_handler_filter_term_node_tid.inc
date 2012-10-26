<?php

/**
 * @file
 * Definition of views_handler_filter_term_node_tid.
 */

/**
 * Filter by term id.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_term_node_tid extends views_handler_filter_many_to_one {
  // Stores the exposed input for this filter.
  var $validated_exposed_input = NULL;

  function init(&$view, &$options) {
    parent::init($view, $options);
    if (!empty($this->definition['vocabulary'])) {
      $this->options['vocabulary'] = $this->definition['vocabulary'];
    }
    $this->options['all_vocabularies'] = _views_taxonomy_get_node_vocabularies();

    // Convert legacy vid option to machine name vocabulary.
    if (isset($this->options['vid']) && !empty($this->options['vid']) & empty($this->options['vocabulary'])) {
      $vocabularies = $this->options['all_vocabularies'];
      $vid = $this->options['vid'];
      if (isset($vocabularies[$vid], $vocabularies[$vid]->machine_name)) {
        $this->options['vocabulary'] = $vocabularies[$vid]->machine_name;
      }
    }
  }

  function has_extra_options() { return TRUE; }

  function get_value_options() { /* don't overwrite the value options */ }

  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'textfield');
    $options['limit'] = array('default' => TRUE, 'bool' => TRUE);
    $options['vocabulary'] = array('default' => array());
    $options['per_vocabulary'] = array('default' => TRUE);
    $options['hierarchy'] = array('default' => 0);
    $options['error_message'] = array('default' => TRUE, 'bool' => TRUE);

    return $options;
  }

  function extra_options_form(&$form, &$form_state) {
    $vocabularies = $this->options['all_vocabularies'];
    $options = array();
    foreach ($vocabularies as $voc) {
      $options[$voc->machine_name] = check_plain($voc->name);
    }

    if ($this->options['limit']) {
      // We only do this when the form is displayed.
      if (empty($this->options['vocabulary'])) {
        $first_vocabulary = reset($vocabularies);
        $this->options['vocabulary'] = array($first_vocabulary->machine_name => $first_vocabulary->machine_name);
      }

      if (empty($this->definition['vocabulary'])) {
        $form['vocabulary'] = array(
          '#type' => 'checkboxes',
          '#title' => t('Vocabulary'),
          '#options' => $options,
          '#description' => t('Select which vocabulary to show terms for in the regular options.'),
          '#default_value' => $this->options['vocabulary'],
        );
      }
    }

    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Selection type'),
      '#options' => array('select' => t('Dropdown'), 'textfield' => t('Autocomplete')),
      '#default_value' => $this->options['type'],
    );

    $form['per_vocabulary'] = array(
      '#type' => 'checkbox',
      '#title' => t('Each vocabulary separately'),
      '#description' => t('Like independent filter per vocabulary.'),
      '#default_value' => !empty($this->options['per_vocabulary']),
      '#dependency' => array('radio:options[type]' => array('select')),
    );

    $form['hierarchy'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show hierarchy in dropdown'),
      '#default_value' => !empty($this->options['hierarchy']),
      '#dependency' => array('radio:options[type]' => array('select')),
    );
  }

  function value_form(&$form, &$form_state) {
    $voc_names = array_filter($this->options['vocabulary']);
    if (empty($voc_names)) {
      $vocabularies = $this->options['all_vocabularies'];
    }
    else {
      $vocabularies = _views_taxonomy_get_vocabularies_by_names($this->options['vocabulary']);
    }
    if (empty($vocabularies) && $this->options['limit']) {
      $form['markup'] = array(
        '#markup' => '<div class="form-item">' . t('An invalid vocabulary is selected. Please change it in the options.') . '</div>',
      );
      return;
    }

    foreach ($vocabularies as $vocabulary) {
      $vocabularies_names[] = $vocabulary->name;
    }

    if ($this->options['type'] == 'textfield') {
      $default = '';
      if ($this->value) {
        $result = db_select('taxonomy_term_data', 'td')
          ->fields('td')
          ->condition('td.tid', $this->value)
          ->execute();
        foreach ($result as $term) {
          if ($default) {
            $default .= ', ';
          }
          $default .= $term->name;
        }
      }

      $form['value'] = array(
        '#title' => $this->options['limit'] ? t('Select terms from vocabulary @voc', array('@voc' => implode(', ', $vocabularies_names))) : t('Select terms'),
        '#type' => 'textfield',
        '#default_value' => $default,
      );

      if ($this->options['limit']) {
        $form['value']['#autocomplete_path'] = 'admin/views/ajax/autocomplete/taxonomy/' . implode(',', array_keys($vocabularies));
      }
    }
    else {
      $options = array();
      foreach ($vocabularies as $vocabulary) {
        $options[$vocabulary->name] = array();
        $vocabularies_names[] = $vocabulary->name;
        if (!empty($this->options['hierarchy']) && $this->options['limit']) {
          $tree = taxonomy_get_tree($vocabulary->vid);
          if ($tree) {
            foreach ($tree as $term) {
              $choice = new stdClass();
              if (!empty($this->options['per_vocabulary'])) {
                $choice->option = array($term->vid . '-' . $term->tid => str_repeat('-', $term->depth) . $term->name);
              }
              else {
                $choice->option = array($term->tid => str_repeat('-', $term->depth) . $term->name);
              }
              $options[$vocabulary->name][] = $choice;
            }
          }
        }
        else {
          $query = db_select('taxonomy_term_data', 'td');
          $query->innerJoin('taxonomy_vocabulary', 'tv', 'td.vid = tv.vid');
          $query->fields('td');
          $query->orderby('tv.weight');
          $query->orderby('tv.name');
          $query->orderby('td.weight');
          $query->orderby('td.name');
          $query->addTag('term_access');
          if ($this->options['limit']) {
            $query->condition('tv.machine_name', $vocabulary->machine_name);
          }
          $result = $query->execute();
          foreach ($result as $term) {
            if (!empty($this->options['per_vocabulary'])) {
              $options[$vocabulary->name][$term->vid . '-' . $term->tid] = $term->name;
            }
            else {
              $options[$vocabulary->name][$term->tid] = $term->name;
            }
          }
        }
      }

      $default_value = (array) $this->value;

      if (!empty($form_state['exposed'])) {
        $identifier = $this->options['expose']['identifier'];

        if (!empty($this->options['expose']['reduce'])) {
          $options = $this->reduce_value_options($options);

          if (!empty($this->options['expose']['multiple']) && empty($this->options['expose']['required'])) {
            $default_value = array();
          }
        }

        if (empty($this->options['expose']['multiple'])) {
          if (empty($this->options['expose']['required']) && (empty($default_value) || !empty($this->options['expose']['reduce']))) {
            $default_value = 'All';
          }
          elseif (empty($default_value)) {
            $keys = array_keys($options);
            $default_value = array_shift($keys);
          }
          // Due to #1464174 there is a chance that array('') was saved in the admin ui.
          // Let's choose a safe default value.
          elseif ($default_value == array('')) {
            $default_value = 'All';
          }
          else {
            $copy = $default_value;
            $default_value = array_shift($copy);
          }
        }
      }
      $form['value'] = array(
        '#type' => 'select',
        '#title' => $this->options['limit'] ? t('Select terms from vocabulary @voc', array('@voc' => implode(', ', $vocabularies_names))) : t('Select terms'),
        '#multiple' => TRUE,
        '#options' => $options,
        '#size' => min(9, count($options, COUNT_RECURSIVE)),
        '#default_value' => $default_value,
      );

      if (!empty($form_state['exposed']) && isset($identifier) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $default_value;
      }
    }


    if (empty($form_state['exposed'])) {
      // Retain the helper option
      $this->helper->options_form($form, $form_state);
    }
  }

  function value_validate($form, &$form_state) {
    // We only validate if they've chosen the text field style.
    if ($this->options['type'] != 'textfield') {
      return;
    }

    $values = drupal_explode_tags($form_state['values']['options']['value']);
    $tids = $this->validate_term_strings($form['value'], $values);

    if ($tids) {
      $form_state['values']['options']['value'] = $tids;
    }
  }

  function accept_exposed_input($input) {
    if (empty($this->options['exposed'])) {
      return TRUE;
    }

    // If view is an attachment and is inheriting exposed filters, then assume
    // exposed input has already been validated
    if (!empty($this->view->is_attachment) && $this->view->display_handler->uses_exposed()) {
      $this->validated_exposed_input = (array) $this->view->exposed_raw_input[$this->options['expose']['identifier']];
    }

    // If it's non-required and there's no value don't bother filtering.
    if (!$this->options['expose']['required'] && empty($this->validated_exposed_input)) {
      return FALSE;
    }

    $rc = parent::accept_exposed_input($input);
    if ($rc) {
      // If we have previously validated input, override.
      if (isset($this->validated_exposed_input)) {
        $this->value = $this->validated_exposed_input;
      }
    }

    return $rc;
  }

  function exposed_validate(&$form, &$form_state) {
    if (empty($this->options['exposed'])) {
      return;
    }

    $identifier = $this->options['expose']['identifier'];

    // We only validate if they've chosen the text field style.
    if ($this->options['type'] != 'textfield') {
      if ($form_state['values'][$identifier] != 'All')  {
        $this->validated_exposed_input = (array) $form_state['values'][$identifier];
      }
      return;
    }

    if (empty($this->options['expose']['identifier'])) {
      return;
    }

    $values = drupal_explode_tags($form_state['values'][$identifier]);

    $tids = $this->validate_term_strings($form[$identifier], $values);
    if ($tids) {
      $this->validated_exposed_input = $tids;
    }
  }

  /**
   * Validate the user string. Since this can come from either the form
   * or the exposed filter, this is abstracted out a bit so it can
   * handle the multiple input sources.
   *
   * @param $form
   *   The form which is used, either the views ui or the exposed filters.
   * @param $values
   *   The taxonomy names which will be converted to tids.
   *
   * @return array
   *   The taxonomy ids fo all validated terms.
   */
  function validate_term_strings(&$form, $values) {
    if (empty($values)) {
      return array();
    }

    $tids = array();
    $names = array();
    $missing = array();
    foreach ($values as $value) {
      $missing[strtolower($value)] = TRUE;
      $names[] = $value;
    }

    if (!$names) {
      return FALSE;
    }

    $query = db_select('taxonomy_term_data', 'td');
    $query->innerJoin('taxonomy_vocabulary', 'tv', 'td.vid = tv.vid');
    $query->fields('td');
    $query->condition('td.name', $names);
    $query->condition('tv.vid', array_values($this->options['vocabulary']), 'IN');
    $query->addTag('term_access');
    $result = $query->execute();
    foreach ($result as $term) {
      unset($missing[strtolower($term->name)]);
      $tids[] = $term->tid;
    }

    if ($missing && !empty($this->options['error_message'])) {
      form_error($form, format_plural(count($missing), 'Unable to find term: @terms', 'Unable to find terms: @terms', array('@terms' => implode(', ', array_keys($missing)))));
    }
    elseif ($missing && empty($this->options['error_message'])) {
      $tids = array(0);
    }

    return $tids;
  }

  function value_submit($form, &$form_state) {
    // prevent array_filter from messing up our arrays in parent submit.
  }

  function expose_form(&$form, &$form_state) {
    parent::expose_form($form, $form_state);
    if ($this->options['type'] != 'select') {
      unset($form['expose']['reduce']);
    }
    $form['error_message'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display error message'),
      '#default_value' => !empty($this->options['error_message']),
    );
  }

  function admin_summary() {
    // set up $this->value_options for the parent summary
    $this->value_options = array();

    if ($this->value) {
      $this->value = array_filter($this->value);
      $result = db_select('taxonomy_term_data', 'td')
        ->fields('td')
        ->condition('td.tid', $this->value)
        ->execute();
      foreach ($result as $term) {
        $this->value_options[$term->tid] = $term->name;
      }
    }
    return parent::admin_summary();
  }

  function query() {
    // If no filter values are present, then do nothing.
    if (count($this->value) == 0) {
      return;
    }
    elseif (count($this->value) == 1) {
      // Somethis $this->value is an array with a single element so convert it.
      if (is_array($this->value)) {
        $this->value = current($this->value);
      }
      $operator = '=';
    }
    else {
      $operator = 'IN';
    }

    // The normal use of ensure_my_table() here breaks Views.
    // So instead we trick the filter into using the alias of the base table.
    // See http://drupal.org/node/271833
    // If a relationship is set, we must use the alias it provides.
    if (!empty($this->relationship)) {
      $this->table_alias = $this->relationship;
    }
    // If no relationship, then use the alias of the base table.
    elseif (isset($this->query->table_queue[$this->query->base_table]['alias'])) {
      $this->table_alias = $this->query->table_queue[$this->query->base_table]['alias'];
    }
    // This should never happen, but if it does, we fail quietly.
    else {
      return;
    }

    if (empty($this->options['per_vocabulary']) || $operator == '=') {
      if (!is_array($this->value) && $value_parts = explode('-', $this->value)) {
        $tid = $value_parts[1];
      }
      else {
        $tid = $this->value;
      }
      $subquery = db_select('taxonomy_index', 'tn');
      $subquery->addField('tn', 'nid');
      $last = 'tn';
      $subquery->condition('tn.tid', $tid, $operator);
      $this->query->add_where($this->options['group'], "$this->table_alias.nid", $subquery, 'IN');
    }
    else {
      $values = array();
      foreach ($this->value as $value) {
        $value_parts = explode('-', $value);
        $values[$value_parts[0]][] = $value_parts[1];
      }
      foreach ($values as $vid => $tids) {
        // Now build the subqueries.
        $subquery = db_select('taxonomy_index', 'tn');
        $subquery->addField('tn', 'nid');
        $last = 'tn';
        $subquery->innerJoin('taxonomy_term_data', 'td', 'td.tid = tn.tid');
        $last = 'td';
        $subquery->condition('tn.tid', array_values($tids), $operator);
        $subquery->condition('td.vid', $vid);
        $this->query->add_where($this->options['group'], "$this->table_alias.nid", $subquery, 'IN');
      }
    }
  }
}