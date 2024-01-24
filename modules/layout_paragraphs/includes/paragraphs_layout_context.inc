<?php
/**
 * @file
 * Layout context handler for entities.
 */
class ParagraphsLayoutContext extends LayoutContext {
  var $usageType = LayoutContext::USAGE_TYPE_SYSTEM;

  /**
   * Return the indicator for this context.
   */
  function type() {
    return 'paragraphs';
  }

  /**
   * Assemble a human-readable label of this object.
   */
  function label() {
    return t('Paragraphs');
  }
}
