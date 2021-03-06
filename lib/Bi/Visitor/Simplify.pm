=head1 NAME

Bi::Visitor::Simplify - simplify expression.

=head1 SYNOPSIS

    use Bi::Visitor::Simplify;
    my $simpler = Bi::Visitor::ToPerl->evaluate($expr);

=head1 INHERITS

L<Bi::Visitor>

=head1 METHODS

=over 4

=cut

package Bi::Visitor::Simplify;

use parent 'Bi::Visitor';
use warnings;
use strict;

use Bi::Expression;

=item B<evaluate>(I<expr>)

Evaluate.

=over 4

=item I<expr> L<Bi::Expression> object.

=back

Returns the expression as a Perl string.

=cut
sub evaluate {
    my $class = shift;
    my $expr = shift;
    
    my $self = new Bi::Visitor; 
    bless $self, $class;
    
    return $expr->accept($self);
}

=item B<visit_after>(I<node>)

Visit node of expression tree.

=cut
sub visit_after {
    my $self = shift;
    my $node = shift;
    
    if ($node->isa('Bi::Expression::BinaryOperator')) {
        if ($node->get_op eq '+') {
            if ($node->get_expr1->is_const && $node->get_expr1->eval_const == 0.0) {
                $node = $node->get_expr2;
            } elsif ($node->get_expr2->is_const && $node->get_expr2->eval_const == 0.0) {
                $node = $node->get_expr1;
            }
        } elsif ($node->get_op eq '-') {
            if ($node->get_expr1->is_const && $node->get_expr1->eval_const == 0.0) {
                $node = new Bi::Expression::UnaryOperator('-', $node->get_expr2);
            } elsif ($node->get_expr2->is_const && $node->get_expr2->eval_const == 0.0) {
                $node = $node->get_expr1;
            }
        } elsif ($node->get_op eq '*') {
            if ($node->get_expr1->is_const && $node->get_expr1->eval_const == 1.0) {
                $node = $node->get_expr2;
            } elsif ($node->get_expr2->is_const && $node->get_expr2->eval_const == 1.0) {
                $node = $node->get_expr1;
            } elsif ($node->get_expr1->is_const && $node->get_expr1->eval_const == 0.0) {
                $node = $node->get_expr1;
            } elsif ($node->get_expr2->is_const && $node->get_expr2->eval_const == 0.0) {
                $node = $node->get_expr2;
            }
        } elsif ($node->get_op eq '/') {
            if ($node->get_expr2->is_const && $node->get_expr2->eval_const == 1.0) {
                $node = $node->get_expr1;
            }
        }
    } elsif ($node->isa('Bi::Expression::Function')) {
        if ($node->get_name eq 'exp') {
            my $arg = $node->get_arg(0);
            if ($arg->isa('Bi::Expression::Function') && $arg->get_name eq 'log') {
                $node = $arg->get_arg(0);
            }
        } elsif ($node->get_name eq 'log') {
            my $arg = $node->get_arg(0);
            if ($arg->isa('Bi::Expression::Function') && $arg->get_name eq 'exp') {
                $node = $arg->get_arg(0);
            }
        }
    }
    return $node;
}

1;

=back

=head1 SEE ALSO

L<Bi::Expression>

=head1 AUTHOR

Lawrence Murray <lawrence.murray@csiro.au>

