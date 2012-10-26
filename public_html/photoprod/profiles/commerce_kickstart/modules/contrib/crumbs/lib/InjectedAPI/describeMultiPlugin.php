<?php


/**
 * Injected API object for the describe() method of multi plugins.
 */
class crumbs_InjectedAPI_describeMultiPlugin {

  protected $invokeAction;

  function __construct($invoke_action) {
    $this->invokeAction = $invoke_action;
  }

  function addRule($key_suffix, $title = TRUE) {
    $this->invokeAction->addRule($key_suffix, $title);
  }
}
