#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#define PI 3.14159265358979323846

struct stack_rec
{
    char *stack1;
    char *stack2;
    int capacity;
    int top1, top2;
};

struct result
{
    int top;
    double *result;
    int capacity;
};

typedef struct stack_rec *stack;
stack s;
typedef struct result *result;
result r;

void create_stack(int c)
{
    s = (stack)malloc(sizeof(struct stack_rec));
    if(s == NULL)
    {
        printf("Size not allocated:\n");
    }
    if(c < 1)
    {
        printf("Fatal error\n");
    }
    else
    {
        s->stack1 = (char*)malloc(sizeof(char) * c);
        if(s->stack1 == NULL)
        {
            printf("Memory not allocated for the stack1:\n");
        }
        s->stack2 = (char*)malloc(sizeof(char) * (c * 2));
        if(s->stack2 == NULL)
        {
            printf("Memory not allocated for the stack2\n");
        }
        else
        {
            s->top1 = -1;
            s->top2 = -1;
            s->capacity = c * 2;
            printf("Stack created successfully...........\n");
        }
    }
}

void create_result(int length)
{
    r = (result)malloc(sizeof(struct result));
    if(r == NULL)
    {
        printf("Memory allocation failed for struct pointer\n");
        return;
    }
    else
    {
        r->result = (double*)malloc(sizeof(double) * length);
        if(r->result == NULL)
        {
            printf("Memory allocation failed for result array\n");
            return;
        }
        printf("Result stack created successfully......\n");
        r->top = -1;
        r->capacity = length;
    }
}

void push1(char ch)
{
    if(s->top1 < s->capacity - 1)
    {
        s->stack1[++s->top1] = ch;
    }
}

void push2(char ch)
{
    if(s->top2 < s->capacity - 1)
    {
        s->stack2[++s->top2] = ch;
    }
}

void push_result(double val)
{
    if(r->top < r->capacity - 1)
    {
        r->result[++r->top] = val;
    }
}

double pop_result()
{
    if(r->top >= 0)
    {
        return r->result[r->top--];
    }
    return 0.0;
}

char peek1()
{
    if(s->top1 >= 0)
    {
        return s->stack1[s->top1];
    }
    return '\0';
}

char pop1()
{
    if(s->top1 >= 0)
    {
        return s->stack1[s->top1--];
    }
    return '\0';
}

int precedence(char ch)
{
    if(ch == '+' || ch == '-')
    {
        return 1;
    }
    else if(ch == '*' || ch == '/' || ch == '%')
    {
        return 2;
    }
    else if(ch == '^' || ch == 's' || ch == 'c' || ch == 't' || ch == 'l' || ch == 'q')
    {
        return 3;
    }
    else 
        return 0;
}

int main()
{
    printf("\n");
    int length, i, j = 1,m=0,n=0,o=0,p=0,q=0;
    double val, a, b, answer;
    char expression[1024], current;
    printf("\tExpression evaluator with Scientific calculator:\n");
    printf("----------------------------------------------------------------\n");
    printf("Use words as follows - sin , cos , tan , log , sqrt\n");
    printf("Enter the expression which you want to evaluate: ");
    if(fgets(expression, sizeof(expression), stdin) == NULL)
    {
        return 0;
    }
    expression[strcspn(expression, "\n")] = 0;
    length = strlen(expression);
    
    create_stack(length * 2);
    create_result(length);

    for(i = 0; i < length; i++)
    {
        current = expression[i];
        if(isspace(current))
        {
            continue;
        }
        if(isdigit(current) || current == '.')
        {
            while(i < length && (isdigit(expression[i]) || expression[i] == '.'))
            {
                push2(expression[i]);
                i++;
            }
            i--;
            push2(' ');
        }
        else if(isalpha(current))
        {
            char word[10];
            int idx = 0;
            while(i < length && isalpha(expression[i]))
            {
                word[idx++] = expression[i];
                i++;
            }
            word[idx] = '\0';
            i--;

            if(strcmp(word, "sin") == 0 || strcmp(word, "cos") == 0 || strcmp(word, "tan") == 0 || strcmp(word, "sqrt") == 0 || strcmp(word, "log") == 0)
            {
                while(s->top1 >= 0 && peek1() != '(' && precedence(peek1()) >= 3)
                {
                    push2(pop1());
                    push2(' ');
                }
                if(strcmp(word, "sin") == 0) push1('s');
                else if(strcmp(word, "cos") == 0) push1('c');
                else if(strcmp(word, "tan") == 0) push1('t');
                else if(strcmp(word, "log") == 0) push1('l');
                else if(strcmp(word, "sqrt") == 0) push1('q');
            }
            else
            {
                for(int k = 0; k < idx; k++)
                {
                    push2(word[k]);
                }
                push2(' ');
            }
        }
        else if(current == '(')
        {
            push1(current);
        }
        else if(current == ')')
        {
            while(s->top1 >= 0 && peek1() != '(')
            {
                push2(pop1());
                push2(' ');
            }
            if(s->top1 >= 0 && peek1() == '(')
            {
                pop1();
            }
        }
        else
        {
            while(s->top1 >= 0 && peek1() != '(' && precedence(peek1()) >= precedence(current))
            {
                push2(pop1());
                push2(' ');
            }
            push1(current);
        }
    }
    
    while (s->top1 >= 0)
    {
        push2(' ');
        push2(pop1());
    }
    push2('\0');
    
    printf("Postfix expression: %s \n", s->stack2);

    for(i = 0; i <= s->top2; i++)
    {
        current = s->stack2[i];
        if(current == '\0')
        {
            break;
        }
        if(current == ' ')
        {
            continue;
        }
        
        if(isdigit(current) || current == '.')
        {
            char num[64];
            int index = 0;
            while(i <= s->top2 && (isdigit(s->stack2[i]) || s->stack2[i] == '.'))
            {
                num[index++] = s->stack2[i];
                i++;
            }
            num[index] = '\0';
            i--;
            push_result(atof(num));
        } 
        else if(isalpha(current))
        {
            if(current == 's' || current == 'c' || current == 't' || current == 'l')
            {
                a = pop_result();
                if(current == 's') val = sin(a * PI / 180.0);
                else if(current == 'c') val = cos(a * PI / 180.0);
                else if(current == 't') val = tan(a * PI / 180.0);
                push_result(val);
            }
            else if(current == 'q')
            {
                a = pop_result();
                if(a < 0)
                {
                    printf("Error: Negative value for square root is not accepted\n");
                    val = 0.0;
                    m=1;
                }
                else
                {
                    val = sqrt(a);
                }
                push_result(val);
            }
            else if(current =='l')
            {
                a=pop_result();
                if(a<0)
                {
                    printf("Error: Negative value for logarithm input is not accepted\n");
                    val=0.0;
                    q=1;
                }
                else
                {
                    val=log10(a);
                }
                push_result(val);
            }
            else
            {
                printf("Enter value for variable %c: %d  ", current, j++);
                scanf("%lf", &val);
                push_result(val);
            }
        }   
        else
        {
            a = pop_result();
            b = pop_result();
            
            switch(current)
            {
                case '+':
                    val = b + a;
                    push_result(val);
                    break;

                case '-':
                    val = b - a;
                    push_result(val);
                    break;

                case '*':
                    val = b * a;
                    push_result(val);
                    break;

                case '/':
                    if (a == 0)
                    {
                        printf("Error: Division with zero is not possible\n");
                        val = 0.0;
                        n=1;
                    }
                    else
                    {
                        val = b / a;
                    }
                    push_result(val);
                    break;

                case '^':
                    val = pow(b, a);
                    push_result(val);
                    break;

                case '%':
                    if (a == 0)
                    {
                        printf("Error:Modulo by zero is not possible\n");
                        val = 0.0;
                        o=1;
                    }
                    else
                    {
                        val = fmod(b, a);
                    }
                    push_result(val);
                    break;

                default:
                    printf("Not a valid operator: %c\n", current);
                    p=1;
                    break;
            }
        }
    }
    
    if (r->top >= 0)
    {
        answer = pop_result();
        if(m==1||n==1||o==1||p==1)
        {
            printf("Evaluation error\n");
            printf("----------------------------------------------------------------");
            return 0;
        }
        printf("The answer of the expression entered is : %.4f\n", answer);
        printf("----------------------------------------------------------------");
    }
    else
    {
        printf("Evaluation error.\n");
    }

    return 0;
}