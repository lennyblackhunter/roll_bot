import unittest

from sheet_render.render import _format_ability_name


class FormatAbilityNameTest(unittest.TestCase):
    def test_replaces_underscores_with_spaces(self) -> None:
        self.assertEqual(_format_ability_name("spot_hidden"), "spot hidden")

    def test_inserts_space_before_parenthesis(self) -> None:
        self.assertEqual(_format_ability_name("credit_rating(wealth)"), "credit rating (wealth)")

    def test_preserves_existing_space_before_parenthesis(self) -> None:
        self.assertEqual(_format_ability_name("listen (hard)"), "listen (hard)")


if __name__ == "__main__":
    unittest.main()
