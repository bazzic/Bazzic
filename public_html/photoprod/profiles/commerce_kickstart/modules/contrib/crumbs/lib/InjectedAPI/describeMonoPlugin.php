<?php


/**
 * Injected API object for the describe() method of mono plugins.
 */
class crumbs_InjectedAPI_describeMonoPlugin {

  protected $invokeAction;

  function __construct($invoke_action) {
    $this->invokeAction = $invoke_action;
  }

  function setTitle($title) {
    $this->invokeAction->setTitle($title);
  }
}
